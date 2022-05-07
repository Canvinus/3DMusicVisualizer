#pragma once

#include <string>

#include "model_data.h"

/**
 * Loads the model from the file<br>
 * See the libraries/CMakeLists.txt file for supported formats<br>
 * The importers with the commented lines are enabled
 * @param path the path to the file
 * @return the model data from the file
 */
ModelData loadModel(const std::string& path);
