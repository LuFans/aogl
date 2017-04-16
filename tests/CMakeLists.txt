#
# mutiple samples and test for aogl
#

#triangle test
add_executable(triangle triangle.cpp)
target_include_directories(triangle PRIVATE ../include)
target_link_libraries(triangle aogl)

#quad test
add_executable(quad quad.cpp)
target_include_directories(quad PRIVATE ../include)
target_link_libraries(quad aogl)

#mesh test
add_executable(mesh mesh.cpp)
target_include_directories(mesh PRIVATE ../include)
target_link_libraries(mesh aogl)

#shadow test
add_executable(shadow shadow.cpp)
target_include_directories(shadow PRIVATE ../include)
target_link_libraries(shadow aogl)