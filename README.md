# aogl - Advanced (Object Oriented) OpenGL Wrapper

AOGL wraps around OpenGL and tries to deliver a Framework as modular as using OpenGL directly while speeding up your coding process.

For usage please refer to the test directory where multiple examples show everything you need to learn about aogl

(This Readme is WIP so you should refer to the examples rather than what is written here)

### Comparison 

Creating a shader in raw OpenGL calls

```C++
int main() {

  [...] // Initialize OpenGL 
  
  auto path = "assets/shader/simple.vert"
  
  // Load File into String
  std::string content;
  std::stringstream ss;
  
  std::ifstream file(path, std::ios_base::in);
  if (!file.is_open())
    throw std::runtime_error("failed to open shader file");
  
  ss << file.rdbuf();
  
  content = ss.str();
  
  // Create Shader
  auto shader = glCreateShader(GL_VERTEX_SHADER);
  auto c_content = content.c_str();
  
  glShaderSource(shader, 1, &c_content, nullptr);
  glCompileShader(shader);
  
  // Check for Compilation Error
  {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      int log_length;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
      std::string info_log;
      info_log.reserve(log_length);
      glGetShaderInfoLog(shader, log_length, nullptr, &info_log[0]);
      std::stringstream ss;
      ss << "shader (" << std::move(_type) << ") to compile: \n" << info_log.c_str();
      throw runtime_error(ss.str());
    }
  }
  
  [...] // Use Shader (link it to a Program)
  
  glDestroyShader(shader);
  
}
```

Creating a Shader in aogl

```C++

int main() {
  
  [...] // Initialize aogl
  
  aogl::shader vertex_shader(aogl::shader_type::vertex, aogl::utils::read_from_file("assets/shader/simple.vert"));
  
}

```

### Convention

AOGL follows the naming Convention of the standard library of C++
