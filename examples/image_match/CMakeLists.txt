cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED filesystem)
find_package(OpenCV REQUIRED COMPONENTS core calib3d features2d imgproc highgui)
file(GLOB_RECURSE EXAMPLE_SRC "image_match.cpp")
add_executable(image_match ${EXAMPLE_SRC})

target_include_directories(
image_match
PUBLIC
${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(image_match
	ezsift
	Boost::filesystem
	${OpenCV_LIBS}
)

