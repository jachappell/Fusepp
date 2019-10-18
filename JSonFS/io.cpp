#include "io.h"
#include "file.h"
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <sstream>
#include <string>

Directory parse(boost::property_tree::ptree root) {
  Directory root_dir;

  root_dir.setName(root.get<std::string>("name"));

  BOOST_FOREACH (boost::property_tree::ptree::value_type &v,
                 root.get_child("content")) {
    if (auto t = v.second.get<std::string>("type"); t == "directory") {
      root_dir.addEntry(parse(v.second));
    } else {
      File f;
      f.setName(v.second.get<std::string>("name"));
      f.setContent(v.second.get<std::string>("content"));
      root_dir.addEntry(f);
    }
  }

  return root_dir;
}

Directory load(std::string filename) {
  std::ifstream json_file = std::ifstream(filename);
  std::stringstream ss;
  ss << json_file.rdbuf();
  boost::property_tree::ptree root;
  boost::property_tree::read_json(ss, root);

  return parse(root);
}

boost::property_tree::ptree add(FileSystemObject root_dir) {
  std::cout << "add(FileSystemObject)" << std::endl;
  boost::property_tree::ptree dir;
  return dir;
}

boost::property_tree::ptree add(Directory root_dir) {
  std::cout << "add(Directory)" << std::endl;
  boost::property_tree::ptree dir, children;
  dir.put(boost::property_tree::ptree::path_type{"name"}, root_dir.getName());
  dir.put(boost::property_tree::ptree::path_type{"type"}, "directory");
  for (auto elem : root_dir.getContent()) {
    std::cout << elem.first << std::endl;
    boost::property_tree::ptree child = add(elem.second);
  }
  // dir.put(boost::property_tree::ptree::path_type{"content"}, children);
  return dir;
}

boost::property_tree::ptree add(File root_dir) {
  std::cout << "add(File)" << std::endl;
  boost::property_tree::ptree file;
  return file;
}

void save(Directory root_dir, std::string filename) {
  std::ofstream json_file = std::ofstream(filename);

  boost::property_tree::ptree root = add(root_dir);
  /*
      boost::property_tree::ptree , file, file2, children;

      file.put(boost::property_tree::ptree::path_type{"name"}, "foo.txt");
      file.put(boost::property_tree::ptree::path_type{"type"}, "file");
      file.put(boost::property_tree::ptree::path_type{"content"}, "barfoo");

      file2.put(boost::property_tree::ptree::path_type{"name"}, "bar.txt");
      file2.put(boost::property_tree::ptree::path_type{"type"}, "file");
      file2.put(boost::property_tree::ptree::path_type{"content"}, "!@312323");

      children.push_back(std::make_pair("", file));
      children.push_back(std::make_pair("", file2));
      root.put(boost::property_tree::ptree::path_type{"name"},
     root_dir.getName());
      root.put(boost::property_tree::ptree::path_type{"type"}, "directory");
      root.put_child(boost::property_tree::ptree::path_type{"content"},
     getContent(root_dir));
  */

  std::stringstream ss;
  boost::property_tree::write_json(ss, root);
  std::cout << ss.str() << std::endl;
  json_file << ss.str();
  json_file.close();
}