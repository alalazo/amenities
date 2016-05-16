

ADD_EXECUTABLE(eratosthenes main.cxx)
TARGET_INCLUDE_DIRECTORIES(
  eratosthenes
  PUBLIC
  ${Boost_INCLUDE_DIRS}
)
TARGET_LINK_LIBRARIES(
  eratosthenes
  PUBLIC
  ${Boost_LIBRARIES}
)