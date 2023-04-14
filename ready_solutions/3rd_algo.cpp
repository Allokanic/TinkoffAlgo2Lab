#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

int main() {
  std::cin.tie(0);
  std::ios_base::sync_with_stdio(0);

  int n;
  std::cin >> n;
  if (n <= 0) {
    int m;
    std::cin >> m;
    while (m--) {
      std::cout << 0 << ' ';
    }
    return 0;
  }
  std::vector<std::tuple<int, int, int, int>> rectangles;

  for (int i = 0; i < n; ++i) {
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    rectangles.push_back({x1, y1, x2, y2});
  }

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

  int m;
  std::cin >> m;
  while (m--) {
    int x, y;
    std::cin >> x >> y;
    int answer = 0;
    if (x < compress_x[0] || y < compress_y[0]) {
      answer = 0;
    } else {
      int compressed_px = std::upper_bound(compress_x.begin(), compress_x.end(), x) - compress_x.begin() - 1;
      int compressed_py = std::upper_bound(compress_y.begin(), compress_y.end(), y) - compress_y.begin() - 1;
      auto trt = par[std::upper_bound(comp_idx.begin(), comp_idx.end(), compressed_px) - comp_idx.begin() - 1];
      answer = calc(trt, 0, compress_y.size(), compressed_py);
    }
    std::cout << answer << ' ';
  }
}
