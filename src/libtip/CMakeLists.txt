

add_library(td2_tip
		RgbaImage.cpp

		TipFile.cpp
		TipImage.cpp
		)

target_include_directories(td2_tip PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(td2_tip PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(td2_tip PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		)

# Please use this alias.
add_library(td2::libtip ALIAS td2_tip)