# collectitfs

*[WIP: lexer, parser, and interpreter for a pre-programming language.]*

**collectitfs** es una CLI (Command Line Interface) escrita en C++ que permite recolectar archivos de texto y preprocesarlos para un parseo posterior, imprimiendo estos archivos en una enumeración con un formato y delimitadores especificados por el usuario.

## Dependencias

Para compilar y ejecutar **collectitfs**, necesitas lo siguiente:

- **CMake** (versión 3.31 o superior)
- **Boost** (específicamente el componente `program_options`)
- Un compilador C++ compatible con **C++23** (por ejemplo, clang)

## Compilación e Instalación

Sigue estos pasos para compilar e instalar la CLI:

1. Clona el repositorio:

   ```bash
   git clone https://github.com/gcca/collectitfs.git
   cd collectitfs
   ```

2. Crea un directorio `build` y navega a él:

   ```bash
   mkdir build
   cd build
   ```

3. Configura el proyecto con CMake:

   ```bash
   cmake ..
   ```

   Opcionalmente, puedes habilitar optimizaciones para mejorar el rendimiento:

   ```bash
   cmake .. -DCITFS_ENABLE_OPT=ON
   ```

   Esto activa las banderas `-Ofast`, `-march=native` y `-flto` en configuraciones que no sean de depuración.

4. Compila el proyecto:

   ```bash
   cmake --build .
   ```

5. Instala la CLI:

   ```bash
   cmake --install .
   ```

   Esto instalará `collectitfs` en el directorio `bin` del prefijo de instalación de CMake (por ejemplo, `/usr/local/bin` en sistemas Unix).

## Uso

La CLI acepta varias opciones y argumentos para personalizar su comportamiento. Aquí está la lista completa:

- **`--fmt, -f <formato>`**: Especifica el formato de salida. Por defecto: `"%f:"`. El `%f` será reemplazado por el nombre del archivo.
- **`--brace, -b <delimitador>`**: Especifica el delimitador que envuelve el contenido del archivo. Por defecto: `"```"`.
- **`--help, -h`**: Muestra el mensaje de ayuda y termina la ejecución.
- **`--ignore_dirs`**: Ignora directorios sin reportar errores (por defecto, se reportan como errores).
- **`--ignore_size`**: Ignora archivos que exceden el tamaño máximo de 25KB sin reportar errores (por defecto, se reportan como errores).
- **`paths`**: Rutas a los archivos o directorios a procesar (argumento posicional, obligatorio).

### Ejemplo de Uso

Supongamos que tienes dos archivos de texto, `file1.txt` y `file2.txt`. Puedes ejecutar:

```bash
collectitfs --fmt "Archivo: %f" --brace "***" path/to/file1.txt path/to/file2.txt
```

Esto generará una salida como:

```
Archivo: file1.txt
***
(contenido de file1.txt)
***

Archivo: file2.txt
***
(contenido de file2.txt)
***
```

### Comportamiento

- La CLI verifica que cada archivo sea válido antes de procesarlo. Un archivo es válido si:
  - Existe.
  - No es un directorio (a menos que se use `--ignore_dirs`).
  - Es un archivo regular.
  - Su tamaño no excede 25KB (a menos que se use `--ignore_size`).
  - Puede ser abierto para lectura.
- Si alguna ruta no cumple estas condiciones y no se ignoran los errores, la CLI mostrará mensajes de error y terminará con un código de salida fallido.
- Los archivos válidos se imprimen con el formato y delimitadores especificados.

## Estilo de Código

El proyecto utiliza un estilo de código definido en el archivo `.clang-format`. Se recomienda usar `clang-format` para formatear el código y mantener la consistencia.

## Archivos Ignorados

El archivo `.gitignore` asegura que los siguientes elementos no se incluyan en el repositorio:

- Directorio `build/`
- Directorio `.cache/`
- Archivos con extensión `.json`

## Contribución

Si deseas contribuir al proyecto, por favor:

1. Abre un *issue* en el repositorio de GitHub para reportar problemas o sugerir mejoras.
2. Envía un *pull request* con tus cambios.

¡Gracias por usar **collectitfs**!
