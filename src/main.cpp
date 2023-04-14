#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <tuple>
#include <utility>
#include <vector>

class dual_stream;

std::string parse_file_name(char *str);
void run_first_testcases(dual_stream out, bool run_actions, bool run_buildings);
void run_first_building(dual_stream out);
void run_first_action(dual_stream out);
void run_second_testcases(dual_stream out, bool run_actions, bool run_buildings);
void run_second_building(dual_stream out);
void run_second_action(dual_stream out);
void run_third_testcases(dual_stream out, bool run_actions, bool run_buildings);
void run_third_building(dual_stream out);
void run_third_action(dual_stream out);
std::vector<std::tuple<int, int, int, int>> generate_rectangles(int rects, int step = 25);
std::vector<std::pair<int, int>> generate_points(int points, int step = 25, int bounds_offset = 20);

class dual_stream {
public:
  dual_stream(std::ostream *out_stream_, bool write_to_cout_) : out_stream(*out_stream_), write_to_cout(write_to_cout_) {}

  ~dual_stream() { out_stream.flush(); }

  template <typename T> dual_stream &operator<<(const T &val) {
    if (write_to_cout) {
      std::cout << val;
    }
    out_stream << val;
    return *this;
  }

private:
  std::ostream &out_stream;
  bool write_to_cout;
};

int main(int argc, char *argv[]) {
  bool first = false;
  bool second = false;
  bool third = false;
  bool run_actions = true;
  bool run_buildings = true;
  std::string output_file_name;
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') {
        switch (argv[i][1]) {
        case '1':
          first = true;
          break;
        case '2':
          second = true;
          break;
        case '3':
          third = true;
          break;
        case 'f':
          output_file_name = parse_file_name(argv[i]);
          break;
        case 'b':
          run_actions = false;
          run_buildings = true;
          break;
        case 'a':
          run_actions = true;
          run_buildings = false;
        default:
          std::cout << "Unknown option: " << argv[i] << std::endl;
          break;
        }
      } else {
        std::cout << "Unknown option: " << argv[i] << std::endl;
      }
    }
  } else {
    std::cout << "Please, use these arguments:" << std::endl
              << "\t-1"
              << "\t\trun testcases for the 1st algorithm" << std::endl
              << "\t-2"
              << "\t\trun testcases for the 2nd algorithm" << std::endl
              << "\t-3"
              << "\t\trun testcases for the 3rd algorithm" << std::endl
              << "\t-f=<filename>"
              << "\tsave results to <filename>" << std::endl
              << "\t-b"
              << "\t\trun only building" << std::endl
              << "\t-a"
              << "\t\trun only action" << std::endl;
    return 0;
  }
  std::cout << std::string(100, '=') << std::endl;
  std::cout << "Your choice: " << std::endl;
  std::cout << std::setfill(' ') << std::left << std::setw(20) << "\tFirst:";
  std::cout << (first ? "✅" : "❌") << std::endl;
  std::cout << std::setfill(' ') << std::left << std::setw(20) << "\tSecond:";
  std::cout << (second ? "✅" : "❌") << std::endl;
  std::cout << std::setfill(' ') << std::left << std::setw(20) << "\tThird:";
  std::cout << (third ? "✅" : "❌") << std::endl;
  std::cout << std::endl;
  std::cout << std::setfill(' ') << std::left << std::setw(20) << "\tRun buildings:";
  std::cout << (run_buildings ? "✅" : "❌") << std::endl;
  std::cout << std::setfill(' ') << std::left << std::setw(20) << "\tRun actions:";
  std::cout << (run_actions ? "✅" : "❌") << std::endl;
  std::cout << std::endl;
  std::cout << std::setfill(' ') << std::left << std::setw(20) << "\tSave to file(-f=):";
  std::cout << (output_file_name.empty() ? "❌ (only console output)" : "✅ (" + output_file_name + ")") << std::endl;
  std::cout << std::string(100, '=') << std::endl;

  dual_stream *out;

  std::cout.precision(18);

  if (output_file_name.empty()) {
    out = new dual_stream(&std::cout, false);
  } else {
    auto outf = new std::ofstream(output_file_name);
    outf->precision(18);
    out = new dual_stream(outf, true);
  }

  if (first) {
    run_first_testcases(*out, run_actions, run_buildings);
  }
  if (second) {
    run_second_testcases(*out, run_actions, run_buildings);
  }
  if (third) {
    run_third_testcases(*out, run_actions, run_buildings);
  }
  delete out;
}

std::string parse_file_name(char *str) {
  std::string res;
  int offset = 0;
  while (str[offset]) {
    res += str[offset];
    ++offset;
  }
  // -f=filename
  return res.length() > 3 ? res.substr(3) : "";
}

std::vector<std::tuple<int, int, int, int>> generate_rectangles(int rects, int step) {
  std::vector<std::tuple<int, int, int, int>> res;
  for (int i = 0; i < rects; ++i) {
    res.push_back({step * i, step * i, step * (2 * rects - i), step * (2 * rects - i)});
  }
  return res;
}

std::vector<std::pair<int, int>> generate_points(int points, int step, int bounds_offset) {
  std::vector<std::pair<int, int>> res;
  std::mt19937 rng(0);
  std::uniform_int_distribution<int> rngdis(-bounds_offset, step * 2 * points + bounds_offset);
  for (int i = 0; i < points; ++i) {
    res.push_back({rngdis(rng), rngdis(rng)});
  }
  return res;
}

void run_first_testcases(dual_stream out, bool run_actions, bool run_buildings) {
  out << "🚀Starting testcases for the 1st algorithm..." << '\n';
  if (run_buildings) {
    run_first_building(out);
    out << '\n';
  }
  if (run_actions) {
    run_first_action(out);
    out << '\n';
  }
}

void run_first_building(dual_stream out) {
  out << "\t🛠️ Running building testcases for the 1st algorithm..." << '\n';
  int start = 10;
  int finish = 100000;
  int multiplier = 2;
  int iterations = 100000;
  float iterations_multiplier = 1;
  std::vector<std::pair<int, unsigned long>> results;
  out << "\t🔎Setup: start=" << start << "  finish=" << finish << "  multiplier=" << multiplier << "  iterations=" << iterations
      << "  iterations_multiplier=" << iterations_multiplier << '\n';

  for (int current = start, testcase = 1, current_iterations = iterations; current <= finish;
       current *= multiplier, ++testcase, current_iterations = std::max(1.1f, (current_iterations * iterations_multiplier))) {
    unsigned long nanos_sum = 0;
    out << "\t\t🧪Testcase #" << std::setfill(' ') << std::left << std::setw(8) << testcase << ":"
        << "  current=" << std::setfill(' ') << std::left << std::setw(8) << current << "current_iterations=" << std::setfill(' ') << std::left
        << std::setw(8) << current_iterations;
    auto rectangles = generate_rectangles(current);
    for (int iteration = 0; iteration < current_iterations; ++iteration) {
      auto start_moment = std::chrono::high_resolution_clock::now();
      // START OF ALGORITHM BUILDING
      // END OF ALGORITHM BUILDING
      auto end_moment = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_moment - start_moment);
      nanos_sum += duration.count();
    }
    out << "avg.time=" << nanos_sum / current_iterations << " ns" << '\n';
    results.push_back({current, nanos_sum / current_iterations});
  }
}

void run_first_action(dual_stream out) {
  out << "\t🎬Running action testcases for the 1st algorithm..." << '\n';
  int start = 10;
  int finish = 100000;
  int multiplier = 2;
  int iterations = 100000;
  float iterations_multiplier = 0.6;
  std::vector<std::pair<int, unsigned long>> results;
  out << "\t🔎Setup: start=" << start << "  finish=" << finish << "  multiplier=" << multiplier << "  iterations=" << iterations
      << "  iterations_multiplier=" << iterations_multiplier << '\n';

  for (int current = start, testcase = 1, current_iterations = iterations; current <= finish;
       current *= multiplier, ++testcase, current_iterations = std::max(1.1f, (current_iterations * iterations_multiplier))) {
    unsigned long nanos_sum = 0;
    out << "\t\t🧪Testcase #" << std::setfill(' ') << std::left << std::setw(8) << testcase << ":"
        << "  current=" << std::setfill(' ') << std::left << std::setw(8) << current << "current_iterations=" << std::setfill(' ') << std::left
        << std::setw(8) << current_iterations;
    auto rectangles = generate_rectangles(current);
    auto points = generate_points(current);
    for (int iteration = 0; iteration < current_iterations; ++iteration) {
      int point_idx = iteration % current;
      auto start_moment = std::chrono::high_resolution_clock::now();
      // START OF ALGORITHM ACTION
      int answer = 0;
      for (const auto &r : rectangles) {
        if (std::get<0>(r) <= points[point_idx].first && points[point_idx].first <= std::get<2>(r) && std::get<1>(r) <= points[point_idx].second &&
            points[point_idx].second <= std::get<3>(r)) {
          ++answer;
        }
      }
      // END OF ALGORITHM ACTION
      auto end_moment = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_moment - start_moment);
      nanos_sum += duration.count();
    }
    out << "avg.time=" << nanos_sum / current_iterations << " ns" << '\n';
    results.push_back({current, nanos_sum / current_iterations});
  }
}

void run_second_testcases(dual_stream out, bool run_actions, bool run_buildings) {
  out << "🚀Starting testcases for the 2nd algorithm..." << '\n';
  if (run_buildings) {
    run_second_building(out);
    out << '\n';
  }
  if (run_actions) {
    run_second_action(out);
    out << '\n';
  }
}

void run_second_building(dual_stream out) {
  out << "\t🛠️ Running building testcases for the 2nd algorithm..." << '\n';
  int start = 10;
  int finish = 2000;
  int multiplier = 2;
  int iterations = 10000;
  float iterations_multiplier = 0.2;
  std::vector<std::pair<int, unsigned long>> results;
  out << "\t🔎Setup: start=" << start << "  finish=" << finish << "  multiplier=" << multiplier << "  iterations=" << iterations
      << "  iterations_multiplier=" << iterations_multiplier << '\n';

  for (int current = start, testcase = 1, current_iterations = iterations; current <= finish;
       current *= multiplier, ++testcase, current_iterations = std::max(1.1f, (current_iterations * iterations_multiplier))) {
    unsigned long nanos_sum = 0;
    out << "\t\t🧪Testcase #" << std::setfill(' ') << std::left << std::setw(8) << testcase << ":"
        << "  current=" << std::setfill(' ') << std::left << std::setw(8) << current << "current_iterations=" << std::setfill(' ') << std::left
        << std::setw(8) << current_iterations;
    auto rectangles = generate_rectangles(current);
    for (int iteration = 0; iteration < current_iterations; ++iteration) {
      auto start_moment = std::chrono::high_resolution_clock::now();
      // START OF ALGORITHM BUILDING
      std::vector<int> compress_x;
      std::vector<int> compress_y;
      for (const auto &r : rectangles) {
        compress_x.push_back(std::get<0>(r));
        compress_x.push_back(std::get<2>(r));
        compress_x.push_back(std::get<2>(r) + 1);

        compress_y.push_back(std::get<1>(r));
        compress_y.push_back(std::get<3>(r));
        compress_y.push_back(std::get<3>(r) + 1);
      }
      std::sort(compress_x.begin(), compress_x.end());
      std::sort(compress_y.begin(), compress_y.end());
      compress_x.erase(std::unique(compress_x.begin(), compress_x.end()), compress_x.end());
      compress_y.erase(std::unique(compress_y.begin(), compress_y.end()), compress_y.end());

      std::vector<std::vector<int>> surface;
      surface.resize(compress_x.size());
      for (int i = 0; i < compress_x.size(); ++i) {
        surface[i].resize(compress_y.size());
      }
      for (const auto &r : rectangles) {
        int lbx = std::lower_bound(compress_x.begin(), compress_x.end(), std::get<0>(r)) - compress_x.begin();
        int rtx = std::lower_bound(compress_x.begin(), compress_x.end(), std::get<2>(r)) - compress_x.begin();
        int lby = std::lower_bound(compress_y.begin(), compress_y.end(), std::get<1>(r)) - compress_y.begin();
        int rty = std::lower_bound(compress_y.begin(), compress_y.end(), std::get<3>(r)) - compress_y.begin();
        for (int x = lbx; x <= rtx; ++x) {
          for (int y = lby; y <= rty; ++y) {
            ++surface[x][y];
          }
        }
      }
      // END OF ALGORITHM BUILDING
      auto end_moment = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_moment - start_moment);
      nanos_sum += duration.count();
    }
    out << "avg.time=" << nanos_sum / current_iterations << " ns" << '\n';
    results.push_back({current, nanos_sum / current_iterations});
  }
}

void run_second_action(dual_stream out) {
  out << "\t🎬Running action testcases for the 2nd algorithm..." << '\n';
  int start = 10;
  int finish = 2000;
  int multiplier = 2;
  int iterations = 10000;
  float iterations_multiplier = 0.2;
  std::vector<std::pair<int, unsigned long>> results;
  out << "\t🔎Setup: start=" << start << "  finish=" << finish << "  multiplier=" << multiplier << "  iterations=" << iterations
      << "  iterations_multiplier=" << iterations_multiplier << '\n';

  for (int current = start, testcase = 1, current_iterations = iterations; current <= finish;
       current *= multiplier, ++testcase, current_iterations = std::max(1.1f, (current_iterations * iterations_multiplier))) {
    unsigned long nanos_sum = 0;
    out << "\t\t🧪Testcase #" << std::setfill(' ') << std::left << std::setw(8) << testcase << ":"
        << "  current=" << std::setfill(' ') << std::left << std::setw(8) << current << "current_iterations=" << std::setfill(' ') << std::left
        << std::setw(8) << current_iterations;
    auto rectangles = generate_rectangles(current);
    auto points = generate_points(current);
    // START OF ALGORITHM BUILDING
    std::vector<int> compress_x;
    std::vector<int> compress_y;
    for (const auto &r : rectangles) {
      compress_x.push_back(std::get<0>(r));
      compress_x.push_back(std::get<2>(r));
      compress_x.push_back(std::get<2>(r) + 1);

      compress_y.push_back(std::get<1>(r));
      compress_y.push_back(std::get<3>(r));
      compress_y.push_back(std::get<3>(r) + 1);
    }
    std::sort(compress_x.begin(), compress_x.end());
    std::sort(compress_y.begin(), compress_y.end());
    compress_x.erase(std::unique(compress_x.begin(), compress_x.end()), compress_x.end());
    compress_y.erase(std::unique(compress_y.begin(), compress_y.end()), compress_y.end());

    std::vector<std::vector<int>> surface;
    surface.resize(compress_x.size());
    for (int i = 0; i < compress_x.size(); ++i) {
      surface[i].resize(compress_y.size());
    }
    for (const auto &r : rectangles) {
      int lbx = std::lower_bound(compress_x.begin(), compress_x.end(), std::get<0>(r)) - compress_x.begin();
      int rtx = std::lower_bound(compress_x.begin(), compress_x.end(), std::get<2>(r)) - compress_x.begin();
      int lby = std::lower_bound(compress_y.begin(), compress_y.end(), std::get<1>(r)) - compress_y.begin();
      int rty = std::lower_bound(compress_y.begin(), compress_y.end(), std::get<3>(r)) - compress_y.begin();
      for (int x = lbx; x <= rtx; ++x) {
        for (int y = lby; y <= rty; ++y) {
          ++surface[x][y];
        }
      }
    }
    // END OF ALGORITHM BUILDING
    for (int iteration = 0; iteration < current_iterations; ++iteration) {
      int point_idx = iteration % current;
      auto start_moment = std::chrono::high_resolution_clock::now();
      // START OF ALGORITHM ACTION
      int answer = 0;
      if (points[point_idx].first < compress_x[0] || points[point_idx].second < compress_y[0]) {
        answer = 0;
      } else {
        int compressed_px = std::upper_bound(compress_x.begin(), compress_x.end(), points[point_idx].first) - compress_x.begin() - 1;
        int compressed_py = std::upper_bound(compress_y.begin(), compress_y.end(), points[point_idx].second) - compress_y.begin() - 1;
        answer = surface[compressed_px][compressed_py];
      }
      // END OF ALGORITHM ACTION
      auto end_moment = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_moment - start_moment);
      nanos_sum += duration.count();
    }
    out << "avg.time=" << nanos_sum / current_iterations << " ns" << '\n';
    results.push_back({current, nanos_sum / current_iterations});
  }
}

void run_third_testcases(dual_stream out, bool run_actions, bool run_buildings) {
  out << "🚀Starting testcases for the 3rd algorithm..." << '\n';
  if (run_buildings) {
    run_third_building(out);
    out << '\n';
  }
  if (run_actions) {
    run_third_action(out);
    out << '\n';
  }
}

void run_third_building(dual_stream out) {
  out << "\t🛠️ Running building testcases for the 3rd algorithm..." << '\n';
  int start = 10;
  int finish = 100000;
  int multiplier = 2;
  int iterations = 100000;
  float iterations_multiplier = 0.5;
  std::vector<std::pair<int, unsigned long>> results;
  out << "\t🔎Setup: start=" << start << "  finish=" << finish << "  multiplier=" << multiplier << "  iterations=" << iterations
      << "  iterations_multiplier=" << iterations_multiplier << '\n';

  for (int current = start, testcase = 1, current_iterations = iterations; current <= finish;
       current *= multiplier, ++testcase, current_iterations = std::max(1.1f, (current_iterations * iterations_multiplier))) {
    unsigned long nanos_sum = 0;
    out << "\t\t🧪Testcase #" << std::setfill(' ') << std::left << std::setw(8) << testcase << ":"
        << "  current=" << std::setfill(' ') << std::left << std::setw(8) << current << "current_iterations=" << std::setfill(' ') << std::left
        << std::setw(8) << current_iterations;
    auto rectangles = generate_rectangles(current);
    for (int iteration = 0; iteration < current_iterations; ++iteration) {
      auto start_moment = std::chrono::high_resolution_clock::now();
      // START OF ALGORITHM BUILDING
      std::vector<int> compress_x;
      std::vector<int> compress_y;
      for (const auto &r : rectangles) {
        compress_x.push_back(std::get<0>(r));
        compress_x.push_back(std::get<2>(r));
        compress_x.push_back(std::get<2>(r) + 1);

        compress_y.push_back(std::get<1>(r));
        compress_y.push_back(std::get<3>(r));
        compress_y.push_back(std::get<3>(r) + 1);
      }
      std::sort(compress_x.begin(), compress_x.end());
      std::sort(compress_y.begin(), compress_y.end());
      compress_x.erase(std::unique(compress_x.begin(), compress_x.end()), compress_x.end());
      compress_y.erase(std::unique(compress_y.begin(), compress_y.end()), compress_y.end());

      std::vector<std::tuple<int, bool, int, int>> evs;
      for (const auto &r : rectangles) {
        evs.push_back({std::lower_bound(compress_x.begin(), compress_x.end(), std::get<0>(r)) - compress_x.begin(), true,
                       std::lower_bound(compress_y.begin(), compress_y.end(), std::get<1>(r)) - compress_y.begin(),
                       std::lower_bound(compress_y.begin(), compress_y.end(), std::get<3>(r) + 1) - compress_y.begin()});
        evs.push_back({std::lower_bound(compress_x.begin(), compress_x.end(), std::get<2>(r) + 1) - compress_x.begin(), false,
                       std::lower_bound(compress_y.begin(), compress_y.end(), std::get<1>(r)) - compress_y.begin(),
                       std::lower_bound(compress_y.begin(), compress_y.end(), std::get<3>(r) + 1) - compress_y.begin()});
      }

      std::sort(evs.begin(), evs.end());

      struct v {
        std::shared_ptr<v> l = nullptr;
        std::shared_ptr<v> r = nullptr;
        int s = 0;
      };

      std::function<std::shared_ptr<v>(std::shared_ptr<v>, int, int, int, int, int)> add = [&](std::shared_ptr<v> rt, int l, int r, int tl, int tr,
                                                                                               int val) -> std::shared_ptr<v> {
        if (r <= tl || l >= tr)
          return rt;
        if (tl <= l && r <= tr)
          return std::shared_ptr<v>(new v{rt->l, rt->r, rt->s + val});
        int m = (l + r) / 2;
        return std::shared_ptr<v>(new v{add(rt->l != nullptr ? rt->l : std::shared_ptr<v>(new v), l, m, tl, tr, val),
                                        add(rt->r != nullptr ? rt->r : std::shared_ptr<v>(new v), m, r, tl, tr, val), rt->s});
      };

      std::shared_ptr<v> rt(new v);
      std::vector<std::shared_ptr<v>> par;
      std::vector<int> comp_idx;

      for (int i = 0; i < evs.size(); ++i) {
        if (i)
          if (std::get<0>(evs[i - 1]) != std::get<0>(evs[i])) {
            par.push_back(rt);
            comp_idx.push_back(std::get<0>(evs[i - 1]));
          }
        rt = add(rt, 0, compress_y.size(), std::get<2>(evs[i]), std::get<3>(evs[i]), std::get<1>(evs[i]) ? 1 : -1);
      }
      par.push_back(rt);
      comp_idx.push_back(std::get<0>(evs.back()));
      // END OF ALGORITHM BUILDING
      auto end_moment = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_moment - start_moment);
      nanos_sum += duration.count();
    }
    out << "avg.time=" << nanos_sum / current_iterations << " ns" << '\n';
    results.push_back({current, nanos_sum / current_iterations});
  }
}

void run_third_action(dual_stream out) {
  out << "\t🎬Running action testcases for the 3rd algorithm..." << '\n';
  int start = 10;
  int finish = 100000;
  int multiplier = 2;
  int iterations = 100000;
  float iterations_multiplier = 1;
  std::vector<std::pair<int, unsigned long>> results;
  out << "\t🔎Setup: start=" << start << "  finish=" << finish << "  multiplier=" << multiplier << "  iterations=" << iterations
      << "  iterations_multiplier=" << iterations_multiplier << '\n';

  for (int current = start, testcase = 1, current_iterations = iterations; current <= finish;
       current *= multiplier, ++testcase, current_iterations = std::max(1.1f, (current_iterations * iterations_multiplier))) {
    unsigned long nanos_sum = 0;
    out << "\t\t🧪Testcase #" << std::setfill(' ') << std::left << std::setw(8) << testcase << ":"
        << "  current=" << std::setfill(' ') << std::left << std::setw(8) << current << "current_iterations=" << std::setfill(' ') << std::left
        << std::setw(8) << current_iterations;
    auto rectangles = generate_rectangles(current);
    auto points = generate_points(current);
    // START OF ALGORITHM BUILDING
    std::vector<int> compress_x;
    std::vector<int> compress_y;
    for (const auto &r : rectangles) {
      compress_x.push_back(std::get<0>(r));
      compress_x.push_back(std::get<2>(r));
      compress_x.push_back(std::get<2>(r) + 1);

      compress_y.push_back(std::get<1>(r));
      compress_y.push_back(std::get<3>(r));
      compress_y.push_back(std::get<3>(r) + 1);
    }
    std::sort(compress_x.begin(), compress_x.end());
    std::sort(compress_y.begin(), compress_y.end());
    compress_x.erase(std::unique(compress_x.begin(), compress_x.end()), compress_x.end());
    compress_y.erase(std::unique(compress_y.begin(), compress_y.end()), compress_y.end());

    std::vector<std::tuple<int, bool, int, int>> evs;
    for (const auto &r : rectangles) {
      evs.push_back({std::lower_bound(compress_x.begin(), compress_x.end(), std::get<0>(r)) - compress_x.begin(), true,
                     std::lower_bound(compress_y.begin(), compress_y.end(), std::get<1>(r)) - compress_y.begin(),
                     std::lower_bound(compress_y.begin(), compress_y.end(), std::get<3>(r) + 1) - compress_y.begin()});
      evs.push_back({std::lower_bound(compress_x.begin(), compress_x.end(), std::get<2>(r) + 1) - compress_x.begin(), false,
                     std::lower_bound(compress_y.begin(), compress_y.end(), std::get<1>(r)) - compress_y.begin(),
                     std::lower_bound(compress_y.begin(), compress_y.end(), std::get<3>(r) + 1) - compress_y.begin()});
    }

    std::sort(evs.begin(), evs.end());

    struct v {
      std::shared_ptr<v> l = nullptr;
      std::shared_ptr<v> r = nullptr;
      int s = 0;
    };

    std::function<std::shared_ptr<v>(std::shared_ptr<v>, int, int, int, int, int)> add = [&](std::shared_ptr<v> rt, int l, int r, int tl, int tr,
                                                                                             int val) -> std::shared_ptr<v> {
      if (r <= tl || l >= tr)
        return rt;
      if (tl <= l && r <= tr)
        return std::shared_ptr<v>(new v{rt->l, rt->r, rt->s + val});
      int m = (l + r) / 2;
      return std::shared_ptr<v>(new v{add(rt->l != nullptr ? rt->l : std::shared_ptr<v>(new v), l, m, tl, tr, val),
                                      add(rt->r != nullptr ? rt->r : std::shared_ptr<v>(new v), m, r, tl, tr, val), rt->s});
    };

    std::shared_ptr<v> rt(new v);
    std::vector<std::shared_ptr<v>> par;
    std::vector<int> comp_idx;

    for (int i = 0; i < evs.size(); ++i) {
      if (i)
        if (std::get<0>(evs[i - 1]) != std::get<0>(evs[i])) {
          par.push_back(rt);
          comp_idx.push_back(std::get<0>(evs[i - 1]));
        }
      rt = add(rt, 0, compress_y.size(), std::get<2>(evs[i]), std::get<3>(evs[i]), std::get<1>(evs[i]) ? 1 : -1);
    }
    par.push_back(rt);
    comp_idx.push_back(std::get<0>(evs.back()));

    std::function<int(std::shared_ptr<v>, int, int, int)> calc = [&calc](std::shared_ptr<v> rt, int l, int r, int t) -> int {
      if (l + 1 == r)
        return rt->s;
      int m = (l + r) / 2;
      if (t < m)
        return rt->s + (rt->l != nullptr ? calc(rt->l, l, m, t) : 0);
      else
        return rt->s + (rt->r != nullptr ? calc(rt->r, m, r, t) : 0);
    };
    // END OF ALGORITHM BUILDING
    for (int iteration = 0; iteration < current_iterations; ++iteration) {
      int point_idx = iteration % current;
      auto start_moment = std::chrono::high_resolution_clock::now();
      // START OF ALGORITHM ACTION
      int answer = 0;
      if (points[point_idx].first < compress_x[0] || points[point_idx].second < compress_y[0]) {
        answer = 0;
      } else {
        int compressed_px = std::upper_bound(compress_x.begin(), compress_x.end(), points[point_idx].first) - compress_x.begin() - 1;
        int compressed_py = std::upper_bound(compress_y.begin(), compress_y.end(), points[point_idx].second) - compress_y.begin() - 1;
        auto trt = par[std::upper_bound(comp_idx.begin(), comp_idx.end(), compressed_px) - comp_idx.begin() - 1];
        answer = calc(trt, 0, compress_y.size(), compressed_py);
      }
      // END OF ALGORITHM ACTION
      auto end_moment = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_moment - start_moment);
      nanos_sum += duration.count();
    }
    out << "avg.time=" << nanos_sum / current_iterations << " ns" << '\n';
    results.push_back({current, nanos_sum / current_iterations});
  }
}
