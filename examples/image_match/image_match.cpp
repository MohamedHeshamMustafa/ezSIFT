/*  Copyright (c) 2013, Robert Wang, email: robertwgh (at) gmail.com
    All rights reserved. https://github.com/robertwgh/ezSIFT

    Description:Detect keypoints and extract descriptors from two input images.
                Then, match features from two images using brute-force method.

    Revision history:
        September 15th, 2013: initial version.
        July 2nd, 2018: code refactor.
*/

/*#include "ezsift.h"

#include <iostream>
#include <list>

#define USE_FIX_FILENAME 0
int main(int argc, char *argv[])
{
#if USE_FIX_FILENAME
    char *file1 = "img1.pgm";
    char *file2 = "img2.pgm";
#else
    if (argc != 3) {
        printf("Please input two image filenames.\n");
        printf("usage: image_match img1 img2\n");
        return -1;
    }
    char file1[255];
    char file2[255];
    memcpy(file1, argv[1], sizeof(char) * strlen(argv[1]));
    file1[strlen(argv[1])] = 0;
    memcpy(file2, argv[2], sizeof(char) * strlen(argv[2]));
    file2[strlen(argv[2])] = 0;
#endif

    // Read two input images
    ezsift::Image<unsigned char> image1, image2;
    if (image1.read_pgm(file1) != 0) {
        std::cerr << "Failed to open input image1!" << std::endl;
        return -1;
    }

    if (image2.read_pgm(file2) != 0) {
        printf("Failed to open input image2!\n");
        return -1;
    }
    std::cout << "Image 1 loaded, image size: " << image1.w << " x " << image1.h
              << std::endl;
    std::cout << "Image 2 loaded, image size: " << image2.w << " x " << image2.h
              << std::endl;

    // Double the original image as the first octive.
    ezsift::double_original_image(true);

    // Detect keypoints
    std::list<ezsift::SiftKeypoint> kpt_list1, kpt_list2;
    std::cout << "\nSIFT detection on image 1 ..." << std::endl;
    ezsift::sift_cpu(image1, kpt_list1, true);
    std::cout << "# keypoints in image 1: "
              << static_cast<unsigned int>(kpt_list1.size()) << std::endl;

    std::cout << "\nSIFT detection on image 2 ..." << std::endl;
    ezsift::sift_cpu(image2, kpt_list2, true);
    std::cout << "# keypoints in image2: "
              << static_cast<unsigned int>(kpt_list2.size()) << std::endl;

    // Save keypoint list, and draw keypoints on images.
    ezsift::draw_keypoints_to_ppm_file("sift_keypoints_a.ppm", image1,
                                       kpt_list1);
    ezsift::export_kpt_list_to_file("sift_keypoints_a.key", kpt_list1, true);

    ezsift::draw_keypoints_to_ppm_file("sift_keypoints_b.ppm", image2,
                                       kpt_list2);
    ezsift::export_kpt_list_to_file("sift_keypoints_b.key", kpt_list2, true);

    // Match keypoints.
    std::list<ezsift::MatchPair> match_list;
    ezsift::match_keypoints(kpt_list1, kpt_list2, match_list);

    // Draw result image.
    ezsift::draw_match_lines_to_ppm_file("sift_matching_a_b.ppm", image1,
                                         image2, match_list);
    std::cout << "# of matched keypoints: "
              << static_cast<unsigned int>(match_list.size()) << std::endl;

    return 0;
} */

/*  Copyright (c) 2013, Robert Wang, email: robertwgh (at) gmail.com
    All rights reserved. https://github.com/robertwgh/ezSIFT

    Description:Detect keypoints and extract descriptors from two input images.
                Then, match features from two images using brute-force method.

    Revision history:
        September 15th, 2013: initial version.
        July 2nd, 2018: code refactor.
*/

#include "ezsift.h"

#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

// If this variable is set with 1, will output keypoints and matches else if zero will output only matches
#define OUTPUT_KEY_POINTS 0
using namespace boost::filesystem;

struct path_leaf_string {
    std::string
    operator()(const boost::filesystem::directory_entry &entry) const
    {
        return entry.path().leaf().string();
    }
};

void read_directory(const std::string &name, std::vector<std::string> &v)
{
    boost::filesystem::path p(name);
    boost::filesystem::directory_iterator start(p);
    boost::filesystem::directory_iterator end;
    std::transform(start, end, std::back_inserter(v), path_leaf_string());
}

int main(int argc, char *argv[])
{
    std::cout << "inside main" << std::endl;
    // check input which should be exe file , input directory and output directory
    if (argc != 3) {
        printf("Please enter input and output directories.\n");
        printf("usage: image_match directory\n");
        return -1;
    }


    // Read images in directory
    std::string input_dir_name = argv[1];
    std::string output_dir_name = argv[2];

    boost::filesystem::path dir(output_dir_name);
    if (boost::filesystem::create_directory(dir)) {
        std::cout << "Output directory created" << std::endl;
    }
    std::vector<std::string> dirlist;
    read_directory(input_dir_name, dirlist);
	


    /*DIR *d;
    struct dirent *dir;
    std::vector<std::string> dirlist;
    d = opendir(input_dir_name.c_str());

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) {
                std::string image_name = dir->d_name;
                dirlist.push_back(image_name);
            }
        }

        closedir(d);
    }*/
	//convert from jpg

	for (int i = 0; i < dirlist.size(); i++)
        std::cout << dirlist[i] << std::endl;

    // loop over images
    for (int i = 0; i < dirlist.size(); i += 2) {
        char file1[255];
        char file2[255];

		std::cout << (input_dir_name + dirlist[i]).c_str()  << std::endl;
        memcpy(file1, (input_dir_name + dirlist[i]).c_str(),
               sizeof(char) * strlen((input_dir_name + dirlist[i]).c_str()));
        file1[strlen((input_dir_name + dirlist[i]).c_str())] = 0;
        memcpy(file2, (input_dir_name + dirlist[i + 1]).c_str(),
               sizeof(char) *
                   strlen((input_dir_name + dirlist[i + 1]).c_str()));
        file2[strlen((input_dir_name + dirlist[i + 1]).c_str())] = 0;

        // Read two input images
        ezsift::Image<unsigned char> image1, image2;
        if (image1.read_pgm(file1) != 0) {
            std::cerr << "Failed to open input image1! " << file1 << std::endl;
            return -1;
        }

        if (image2.read_pgm(file2) != 0) {
            printf("Failed to open input image2!\n");
            return -1;
        }
        std::cout << "Image 1 loaded, image size: " << image1.w << " x "
                  << image1.h << std::endl;
        std::cout << "Image 2 loaded, image size: " << image2.w << " x "
                  << image2.h << std::endl;

        // Double the original image as the first octive.
        ezsift::double_original_image(true);

        // Detect keypoints
        std::list<ezsift::SiftKeypoint> kpt_list1, kpt_list2;
        std::cout << "\nSIFT detection on image 1 ..." << std::endl;
        ezsift::sift_cpu(image1, kpt_list1, true);
        std::cout << "# keypoints in image 1: "
                  << static_cast<unsigned int>(kpt_list1.size()) << std::endl;

        std::cout << "\nSIFT detection on image 2 ..." << std::endl;
        ezsift::sift_cpu(image2, kpt_list2, true);
        std::cout << "# keypoints in image2: "
                  << static_cast<unsigned int>(kpt_list2.size()) << std::endl;

        std::string file_name;

        if (OUTPUT_KEY_POINTS) {
            // Save keypoint list, and draw keypoints on images.
            file_name =
                output_dir_name + "sift_keypoints_" + dirlist[i] + ".ppm";
            std::cout << file_name << std::endl;
            ezsift::draw_keypoints_to_ppm_file(file_name.c_str(), image1,
                                               kpt_list1);

            file_name =
                output_dir_name + "sift_keypoints_" + dirlist[i] + ".key";
            ezsift::export_kpt_list_to_file(file_name.c_str(), kpt_list1, true);

            file_name =
                output_dir_name + "sift_keypoints_" + dirlist[i + 1] + ".ppm";
            ezsift::draw_keypoints_to_ppm_file(file_name.c_str(), image2,
                                               kpt_list2);

            file_name =
                output_dir_name + "sift_keypoints_" + dirlist[i + 1] + ".key";
            ezsift::export_kpt_list_to_file(file_name.c_str(), kpt_list2, true);
        }

        // Match keypoints.
        std::list<ezsift::MatchPair> match_list;
        ezsift::match_keypoints(kpt_list1, kpt_list2, match_list);

        // Draw result image.
        file_name = output_dir_name + "sift_matching_" + dirlist[i] + "_" +
                    dirlist[i + 1] + ".key";
        ezsift::draw_match_lines_to_ppm_file(file_name.c_str(), image1, image2,
                                             match_list);
        std::cout << "# of matched keypoints: "
                  << static_cast<unsigned int>(match_list.size()) << std::endl;
    }
    return 0;
}