add_library(
	semant
	env.c
	semant.c
	types.c
	stdlib.c
)
target_link_libraries(semant parser)

add_executable(semanttest main.c)
target_link_libraries(semanttest semant)

add_custom_target(
	run_semanttest
	COMMAND bin=semanttest sh ${PROJECT_SOURCE_DIR}/test.sh
)