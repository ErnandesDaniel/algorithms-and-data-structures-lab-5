#include <bits/stdc++.h>

using namespace std;

// MAX_LOG должен покрывать возможный диапазон значений xor_dist
// Максимальный вес ребра 10^9 (30 бит). В худшем случае xor_dist может быть ~ 2^30-1.
// Но при длинном пути количество рёбер <= n-1 = 199999.
// XOR чисел <= 10^9 может теоретически дать результат <= 2^30 - 1.
// MAX_LOG = 31 достаточно для чисел до 2^31 - 1.
const int MAXN = 200005;
const int MAX_LOG = 31;

vector<pair<int, long long>> g[MAXN];  // Граф: {сосед, вес}
int n, f;
vector<long long> xor_dist;  // XOR-расстояния от f до всех узлов

void dfs_calc(int u, int p, long long dist) {
  xor_dist[u] = dist;
  for (auto& edge : g[u]) {
    int v = edge.first;
    long long w = edge.second;
    if (v != p) {
      dfs_calc(v, u, dist ^ w);
    }
  }
}

struct Trie {
  struct Node {
    int child[2];
    Node() {
      child[0] = child[1] = -1;
    }
  };

  vector<Node> nodes;
  int root;

  Trie() {
    nodes.push_back(Node());
    root = 0;
  }

  void insert(long long num) {
    int cur = root;
    for (int i = MAX_LOG - 1; i >= 0; --i) {
      int b = (num >> i) & 1;
      if (nodes[cur].child[b] == -1) {
        nodes.push_back(Node());
        nodes[cur].child[b] = nodes.size() - 1;
      }
      cur = nodes[cur].child[b];
    }
  }

  long long find_max_xor_with(long long num) {
    long long res = 0;
    int cur = root;
    for (int i = MAX_LOG - 1; i >= 0; --i) {
      int b = (num >> i) & 1;
      int desired = 1 - b;
      if (nodes[cur].child[desired] != -1) {
        res |= (1LL << i);
        cur = nodes[cur].child[desired];
      } else {
        cur = nodes[cur].child[b];
      }
    }
    return res;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n >> f;
  f--;  // Переводим к 0-индексации

  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    u--;
    v--;  // Переводим к 0-индексации
    g[u].push_back({v, w});
    g[v].push_back({u, w});
  }

  xor_dist.assign(n, 0);
  dfs_calc(f, -1, 0LL);

  vector<int> children_of_f;
  for (auto& edge : g[f]) {
    children_of_f.push_back(edge.first);
  }

  long long max_discount = 0;

  if (children_of_f.size() >= 2) {  // Убедимся, что есть хотя бы 2 поддерева
    vector<vector<long long>> subtrees_vals;
    for (int start_node : children_of_f) {
      vector<long long> current_subtree;
      function<void(int, int)> dfs_collect = [&](int u, int p) {
        current_subtree.push_back(xor_dist[u]);
        for (auto& edge : g[u]) {
          int v = edge.first;
          if (v != p && v != f) {
            dfs_collect(v, u);
          }
        }
      };
      dfs_collect(start_node, f);
      if (!current_subtree.empty()) {
        subtrees_vals.push_back(current_subtree);
      }
    }

    // Обработка поддеревьев для поиска макс. XOR
    if (subtrees_vals.size() >= 2) {
      Trie trie;
      // Заполняем бор первым поддеревом
      for (long long val : subtrees_vals[0]) {
        trie.insert(val);
      }

      // Обрабатываем остальные поддеревья
      for (size_t i = 1; i < subtrees_vals.size(); ++i) {
        // Ищем макс. XOR с уже вставленными
        for (long long val : subtrees_vals[i]) {
          long long candidate = trie.find_max_xor_with(val);
          if (candidate > max_discount) {
            max_discount = candidate;
          }
        }
        // Добавляем текущее поддерево в бор
        for (long long val : subtrees_vals[i]) {
          trie.insert(val);
        }
      }
    }
  }

  cout << max_discount << "\n";

  return 0;
}