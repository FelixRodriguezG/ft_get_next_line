*Este proyecto ha sido creado como parte del currículo de 42 por felixrod.*

# get_next_line

## Descripción

`get_next_line` es una función en C que lee un descriptor de archivo **línea a línea**.
Cada llamada devuelve la siguiente línea, incluido su `\n` si lo tiene, o `NULL`
cuando ya no queda nada que leer o se produce un error.

```c
char	*get_next_line(int fd);
```

El objetivo del proyecto es aprender a usar **variables estáticas**: la función
debe recordar entre llamadas lo que ya ha leído pero todavía no ha devuelto.
Solo se pueden usar `read`, `malloc` y `free`.

- **Parte obligatoria:** leer línea a línea de un único fd.
- **Bonus:** usar una sola variable estática y poder alternar entre varios fd
  (leer una línea del 3, otra del 4, otra del 3…) sin que se mezclen.

## Instrucciones

`get_next_line` no es un programa, sino una función que se compila junto a tu propio
`main`. El tamaño de lectura se elige con `-D BUFFER_SIZE=n`. Si no se indica,
vale `1024`.

**Parte obligatoria:**

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

**Bonus:**

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

**Ejemplo de uso:**

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("fichero.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

Cada línea devuelta se reserva con `malloc`, así que **quien llama debe liberarla**
con `free`.

## Algoritmo

### Idea general: un "stash" estático

`read()` no sabe de líneas: lee `BUFFER_SIZE` bytes, que pueden contener media línea
o varias. Por eso se guarda en una variable estática, el **stash**, todo lo leído
que aún no se ha devuelto. En cada llamada:

1. **Leer hasta tener una línea completa** (`ft_read_to_stash`).
   Se llama a `read()` con un buffer de `BUFFER_SIZE` bytes y se añade lo leído al
   final del stash (`ft_strjoin_free`). Se repite hasta que el stash contiene un
   `\n` o `read()` devuelve `0` (fin de fichero). Si `read()` devuelve `-1`, se
   libera todo y se devuelve `NULL`.
2. **Extraer la línea** (`ft_extract_line`).
   Se copia desde el principio del stash hasta el primer `\n`, incluido. Si no hay
   `\n` (última línea del fichero), se copia todo.
3. **Guardar el resto** (`ft_save_rest`).
   Lo que va detrás del `\n` se convierte en el nuevo stash para la siguiente llamada.
   Si no queda nada, el stash se libera y vuelve a `NULL`.

Ejemplo con `BUFFER_SIZE=4` y el fichero `"ab\ncdef\ng"`:

| Llamada | Lecturas con `read()` | Stash antes de extraer | Devuelve | Stash que queda |
|---|---|---|---|---|
| 1 | `"ab\nc"` | `"ab\nc"` | `"ab\n"` | `"c"` |
| 2 | `"def\n"` | `"cdef\n"` | `"cdef\n"` | `NULL` |
| 3 | `"g"`, luego EOF | `"g"` | `"g"` | `NULL` |
| 4 | EOF | `NULL` | `NULL` | `NULL` |

### Por qué este algoritmo

- **Lee lo mínimo necesario.** Se para en cuanto hay un `\n` en el stash, en vez de
  leer el fichero entero y luego trocearlo, como pide el subject. Funciona
  igual con ficheros, stdin o pipes, de los que no se conoce el tamaño.
- **Funciona con cualquier `BUFFER_SIZE`.** Con `1` hace muchas llamadas a
  `read()` pero nunca se pasa de la línea. Con `10000000` lee mucho de una vez y las
  siguientes líneas salen del stash sin volver a leer. Las líneas pueden ser más
  largas que el buffer porque el stash crece concatenando.
- **El buffer de lectura se reserva con `malloc`** y no en la pila, para que un
  `BUFFER_SIZE` muy grande no provoque un *stack overflow*.
- **Sin leaks.** Cada `ft_strjoin_free` libera el stash anterior, y en cualquier
  error (`read` = `-1` o `malloc` fallido) se libera el stash y se deja a `NULL`
  con `ft_free`, para que la siguiente llamada empiece limpia.
- **El stash siempre es `NULL` o tiene contenido.** Nunca se guarda un string
  vacío, lo que simplifica las comprobaciones.

### Bonus: un stash por fd

Con un solo stash, si se alterna entre dos ficheros, el resto de uno se mezcla con
el otro. La solución es que la variable estática sea un **array de stashes indexado
por el número de fd**:

```c
static char	*stash[FD_MAX];
```

Sigue siendo **una sola variable estática**, como exige el bonus. Cada fd usa su
casilla `stash[fd]`, de modo que cada fichero continúa donde lo dejó. Las funciones
auxiliares no cambian, porque ya reciben el stash como parámetro. Se añade la
comprobación `fd >= FD_MAX` para no salirse del array. `FD_MAX` vale `1024`, el
límite habitual de ficheros abiertos por proceso (`ulimit -n`).

Las variables estáticas se inicializan a cero automáticamente, así que todas las
casillas empiezan en `NULL` sin necesidad de inicializarlas.

## Recursos

- `man 2 read`, `man 3 malloc`, `man 3 free`, `man 2 open`
- [C reference: storage duration (`static`)](https://en.cppreference.com/w/c/language/storage_duration)
- [Linux man page de `open(2)` y descriptores de archivo](https://man7.org/linux/man-pages/man2/open.2.html)
- Valgrind para comprobar fugas de memoria: <https://valgrind.org/docs/manual/quick-start.html>

### Uso de IA

Se ha usado IA (Claude y Gemini) con dos fines:

- **Mejorar la comprensión del ejercicio, como guía socrática.** En lugar de pedirle
  el código hecho, lo he usado para entender los conceptos y razonar sobre mi propio
  código: las variables estáticas y su inicialización, el EOF y Ctrl+D en la terminal,
  la evaluación en cortocircuito de `||`, por qué sumar a un puntero `NULL` es
  comportamiento indefinido, o cómo gestionar varios fd en el bonus con `stash[fd]`.
- **Generar tests y archivos de prueba.** Los programas de prueba (`main.c` y
  `main_bonus.c`) y los ficheros de texto con los que he probado distintos
  `BUFFER_SIZE`, líneas vacías, líneas largas, la última línea sin `\n`, varios fd
  alternados y fugas de memoria con Valgrind. Estos ficheros no forman parte de la
  entrega.
