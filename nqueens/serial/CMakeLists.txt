# Iterative solution checking all the n! permutations
ADD_EXECUTABLE(nqueens-iterative-serial main.cxx)
TARGET_INCLUDE_DIRECTORIES(
  nqueens-iterative-serial
  PUBLIC
  ${Boost_INCLUDE_DIRS}
)
TARGET_LINK_LIBRARIES(
  nqueens-iterative-serial
  PUBLIC
  ${Boost_LIBRARIES}
)
INSTALL(
  TARGETS nqueens-iterative-serial 
  DESTINATION bin
)

# Backtracking solution
ADD_EXECUTABLE(nqueens-backtracking-serial main_backtracking.cxx)
TARGET_INCLUDE_DIRECTORIES(
  nqueens-backtracking-serial
  PUBLIC
  ${Boost_INCLUDE_DIRS}
)
TARGET_LINK_LIBRARIES(
  nqueens-backtracking-serial
  PUBLIC
  ${Boost_LIBRARIES}
)
INSTALL(
  TARGETS nqueens-backtracking-serial 
  DESTINATION bin
)
