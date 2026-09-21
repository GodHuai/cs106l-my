/*
 * CS106L Assignment 2: Marriage Pact
 * Created by Haven Whitney with modifications by Fabio Ibanez & Jacob Roberts-Baca.
 *
 * Welcome to Assignment 2 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 */

#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>

std::string kYourName = "God Huai"; // Don't forget to change this!

/**
 * Takes in a file name and returns a set containing all of the applicant names as a set.
 *
 * @param filename  The name of the file to read.
 *                  Each line of the file will be a single applicant's name.
 * @returns         A set of all applicant names read from the file.
 *
 * @remark Feel free to change the return type of this function (and the function
 * below it) to use a `std::unordered_set` instead. If you do so, make sure
 * to also change the corresponding functions in `utils.h`.
 */
std::set<std::string> get_applicants(std::string filename) {
  // STUDENT TODO: Implement this function.
  std::ifstream ifs(filename);
  std::set<std::string> applicants;
  if (!ifs.is_open())
  {
    std::cout << "Error opening file" << std::endl;
    return applicants;
  }
  std::string line;
  while (std::getline(ifs, line))
  {
    applicants.insert(line);
  }
  return applicants;
}

/**
 * Takes in a set of student names by reference and returns a queue of names
 * that match the given student name.
 *
 * @param name      The returned queue of names should have the same initials as this name.
 * @param students  The set of student names.
 * @return          A queue containing pointers to each matching name.
 */
/**
 * Computes the initials of a full name, e.g. "Grace Hopper" -> "GH".
 *
 * Returns an empty string when the name is not exactly two words, which
 * mirrors the autograder's `get_initials` (it returns None in that case).
 */
std::string get_initials(const std::string& name) {
  std::istringstream iss(name);
  std::string first, last, extra;
  if (!(iss >> first >> last)) return "";  // fewer than two words
  if (iss >> extra) return "";             // more than two words
  return std::string(1, first[0]) + last[0];
}

std::queue<const std::string*> find_matches(std::string name, std::set<std::string>& students) {
  std::queue<const std::string*> matches;

  const std::string target = get_initials(name);
  if (target.empty()) return matches;

  // Names sharing the same initials are NOT contiguous in the set's ordering
  // (e.g. "Gabriel Ho" and "Greta Horn" are far apart), so a lower_bound /
  // prefix scan does not apply here -- every student has to be checked.
  for (const std::string& student : students) {
    if (get_initials(student) == target) {
      matches.push(&student);  // &*iterator: the iterator is not itself a pointer
    }
  }

  return matches;
}

/**
 * Takes in a queue of pointers to possible matches and determines the one true match!
 *
 * You can implement this function however you'd like, but try to do something a bit
 * more complicated than a simple `pop()`.
 *
 * @param matches The queue of possible matches.
 * @return        Your magical one true love.
 *                Will return "NO MATCHES FOUND." if `matches` is empty.
 */
std::string get_match(std::queue<const std::string*>& matches) {
  if (matches.empty()) {
    return "NO MATCHES FOUND.";
  }

  // Reservoir sampling (k = 1): pick one candidate uniformly at random in a
  // single pass, without copying the queue into another container.
  // A queue can only be consumed from the front, which fits this pattern well.
  static std::mt19937 rng(std::random_device{}());

  const std::string* chosen = nullptr;
  for (std::size_t seen = 1; !matches.empty(); ++seen) {
    const std::string* current = matches.front();
    matches.pop();
    // The seen-th element replaces the current pick with probability 1/seen.
    if (std::uniform_int_distribution<std::size_t>(1, seen)(rng) == 1) {
      chosen = current;
    }
  }

  return *chosen;
}

/* #### Please don't remove this line! #### */
#include "autograder/utils.hpp"
