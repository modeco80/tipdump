add_executable(tipdump
		main.cpp
		)

set_target_properties(tipdump PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		)

target_link_libraries(tipdump PRIVATE td2::libtip)
