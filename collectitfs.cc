#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

std::string ReplaceAll(const std::string &str, const std::string &from,
                       const std::string &to) {
  std::string result = str;
  std::size_t pos = 0;
  while ((pos = result.find(from, pos)) != std::string::npos) {
    result.replace(pos, from.length(), to);
    pos += to.length();
  }
  return result;
}

bool IsValidTextFile(const std::filesystem::path &path, const bool ignore_dirs,
                     const bool ignore_size,
                     std::vector<std::string> &invalid_paths) {
  static constexpr std::uintmax_t kb_size = 25;
  static constexpr std::uintmax_t max_size = kb_size * 1024;

  try {
    if (!std::filesystem::exists(path)) {
      invalid_paths.emplace_back("[Error: El archivo '" + path.string() +
                                 "' no existe]");
      return false;
    }
    if (std::filesystem::is_directory(path)) {
      if (ignore_dirs) {
        return false;
      } else {
        invalid_paths.emplace_back("[Error: '" + path.string() +
                                   "' es un directorio]");
        return false;
      }
    }
    if (!std::filesystem::is_regular_file(path)) {
      invalid_paths.emplace_back("[Error: '" + path.string() +
                                 "' no es un archivo regular]");
      return false;
    }
    if (std::filesystem::file_size(path) > max_size) {
      if (ignore_size) {
        return false;
      } else {
        invalid_paths.emplace_back("[Error: '" + path.string() + "' excede " +
                                   std::to_string(kb_size) + "KB]");
        return false;
      }
    }
    std::ifstream test_file(path);
    if (!test_file) {
      invalid_paths.emplace_back("[Error: No se puede abrir '" + path.string() +
                                 "' para lectura]");
      return false;
    }
    return true;
  } catch (const std::filesystem::filesystem_error &e) {
    invalid_paths.emplace_back("[Error: Excepción de filesystem en '" +
                               path.string() + "': " + e.what() + "]");
    return false;
  }
}

int main(int argc, char *argv[]) {
  boost::program_options::options_description desc("Opciones permitidas");
  desc.add_options()(
      "fmt,f",
      boost::program_options::value<std::string>()->default_value("%f:"),
      "Formato de salida")(
      "brace,b",
      boost::program_options::value<std::string>()->default_value("```"),
      "Delimitador de contenido")("help,h", "Muestra este mensaje de ayuda")(
      "paths",
      boost::program_options::value<std::vector<std::string>>()->multitoken(),
      "Rutas a procesar")("ignore_dirs", boost::program_options::bool_switch(),
                          "Ignorar directorios sin reportar errores")(
      "ignore_size", boost::program_options::bool_switch(),
      "Ignorar archivos que exceden el tamaño máximo sin reportar errores");

  boost::program_options::positional_options_description pos;
  pos.add("paths", -1);

  boost::program_options::variables_map vm;
  try {
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(desc)
            .positional(pos)
            .run(),
        vm);
    boost::program_options::notify(vm);
  } catch (const boost::program_options::error &e) {
    std::cout << "Error al parsear argumentos: " << e.what() << "\n"
              << desc << std::flush;
    return EXIT_FAILURE;
  }

  if (vm.count("help")) {
    std::cout << desc << std::flush;
    return EXIT_SUCCESS;
  }

  const std::string fmt = vm["fmt"].as<std::string>();
  const std::string brace = vm["brace"].as<std::string>();
  const bool ignore_dirs = vm["ignore_dirs"].as<bool>();
  const bool ignore_size = vm["ignore_size"].as<bool>();

  if (!vm.count("paths")) {
    std::cout << "No se proporcionaron rutas." << std::flush;
    return EXIT_FAILURE;
  }

  const std::vector<std::string> input_paths =
      vm["paths"].as<std::vector<std::string>>();
  std::vector<std::filesystem::path> valid_paths;
  std::vector<std::string> invalid_paths;
  valid_paths.reserve(input_paths.size());

  for (const auto &p : input_paths) {
    const std::filesystem::path path(p);
    if (IsValidTextFile(path, ignore_dirs, ignore_size, invalid_paths)) {
      valid_paths.push_back(path);
    }
  }

  if (!invalid_paths.empty()) {
    for (const auto &error : invalid_paths) { std::cout << error << "\n"; }
    std::cout << std::flush;
    return EXIT_FAILURE;
  }

  for (const auto &path : valid_paths) {
    std::cout << ReplaceAll(fmt, "%f", path.filename().string()) << "\n"
              << brace << "\n";
    std::ifstream file(path);
    std::cout << file.rdbuf() << brace << "\n\n";
  }
  std::cout << std::flush;

  return EXIT_SUCCESS;
}
