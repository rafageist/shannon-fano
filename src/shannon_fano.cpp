#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Symbol {
    std::size_t original_index{};
    std::string name;
    double weight{};
    double probability{};
    std::string code;
};

std::vector<double> prefix_probabilities(const std::vector<Symbol>& symbols) {
    std::vector<double> prefix(symbols.size() + 1, 0.0);
    for (std::size_t i = 0; i < symbols.size(); ++i) {
        prefix[i + 1] = prefix[i] + symbols[i].probability;
    }
    return prefix;
}

double interval_sum(const std::vector<double>& prefix, std::size_t begin, std::size_t end) {
    return prefix[end] - prefix[begin];
}

std::size_t best_split(const std::vector<double>& prefix, std::size_t begin, std::size_t end) {
    if (end - begin < 2) {
        throw std::logic_error("Cannot split an interval with fewer than two symbols.");
    }

    std::size_t best = begin + 1;
    double best_difference = std::numeric_limits<double>::infinity();

    for (std::size_t split = begin + 1; split < end; ++split) {
        const double left = interval_sum(prefix, begin, split);
        const double right = interval_sum(prefix, split, end);
        const double difference = std::abs(left - right);

        if (difference < best_difference) {
            best = split;
            best_difference = difference;
        }
    }

    return best;
}

std::vector<Symbol> make_symbols(const std::vector<double>& weights) {
    if (weights.size() < 2) {
        throw std::invalid_argument("At least two symbols are required.");
    }

    const double total = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (total <= 0.0) {
        throw std::invalid_argument("The sum of all weights must be positive.");
    }

    std::vector<Symbol> symbols;
    symbols.reserve(weights.size());

    for (std::size_t i = 0; i < weights.size(); ++i) {
        if (weights[i] <= 0.0) {
            throw std::invalid_argument("All weights/probabilities must be greater than zero.");
        }

        symbols.push_back(Symbol{
            i,
            "S" + std::to_string(i + 1),
            weights[i],
            weights[i] / total,
            "",
        });
    }

    std::stable_sort(symbols.begin(), symbols.end(), [](const Symbol& a, const Symbol& b) {
        return a.probability > b.probability;
    });

    return symbols;
}

std::vector<Symbol> shannon_fano(const std::vector<double>& weights) {
    std::vector<Symbol> symbols = make_symbols(weights);
    const std::vector<double> prefix = prefix_probabilities(symbols);

    std::vector<std::pair<std::size_t, std::size_t>> pending;
    pending.push_back({0, symbols.size()});

    while (!pending.empty()) {
        const auto [begin, end] = pending.back();
        pending.pop_back();

        if (end - begin < 2) {
            continue;
        }

        const std::size_t split = best_split(prefix, begin, end);

        for (std::size_t i = begin; i < split; ++i) {
            symbols[i].code.push_back('1');
        }
        for (std::size_t i = split; i < end; ++i) {
            symbols[i].code.push_back('0');
        }

        pending.push_back({split, end});
        pending.push_back({begin, split});
    }

    return symbols;
}

double entropy(const std::vector<Symbol>& symbols) {
    double value = 0.0;
    for (const Symbol& symbol : symbols) {
        value += -symbol.probability * std::log2(symbol.probability);
    }
    return value;
}

double expected_code_length(const std::vector<Symbol>& symbols) {
    double value = 0.0;
    for (const Symbol& symbol : symbols) {
        value += symbol.probability * static_cast<double>(symbol.code.size());
    }
    return value;
}

std::vector<double> read_weights_from_stdin() {
    std::size_t count = 0;
    std::cout << "Number of symbols: ";
    if (!(std::cin >> count)) {
        throw std::runtime_error("Invalid symbol count.");
    }

    std::vector<double> weights(count);
    for (std::size_t i = 0; i < count; ++i) {
        std::cout << "Weight/probability for S" << (i + 1) << ": ";
        if (!(std::cin >> weights[i])) {
            throw std::runtime_error("Invalid numeric value.");
        }
    }

    return weights;
}

std::vector<double> read_weights_from_args(int argc, char* argv[]) {
    std::vector<double> weights;
    weights.reserve(static_cast<std::size_t>(argc - 1));

    for (int i = 1; i < argc; ++i) {
        weights.push_back(std::stod(argv[i]));
    }

    return weights;
}

void print_result(const std::vector<Symbol>& symbols) {
    std::cout << "\nShannon-Fano coding\n";
    std::cout << "-------------------\n";
    std::cout << std::left << std::setw(8) << "Symbol"
              << std::right << std::setw(14) << "Probability"
              << std::setw(12) << "Code"
              << std::setw(10) << "Length" << '\n';

    for (const Symbol& symbol : symbols) {
        std::cout << std::left << std::setw(8) << symbol.name
                  << std::right << std::setw(14) << std::fixed << std::setprecision(6) << symbol.probability
                  << std::setw(12) << symbol.code
                  << std::setw(10) << symbol.code.size() << '\n';
    }

    std::cout << "\nEntropy:              " << std::fixed << std::setprecision(6)
              << entropy(symbols) << " bits/symbol\n";
    std::cout << "Expected code length: " << std::fixed << std::setprecision(6)
              << expected_code_length(symbols) << " bits/symbol\n";
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        const std::vector<double> weights = argc > 1
            ? read_weights_from_args(argc, argv)
            : read_weights_from_stdin();

        const std::vector<Symbol> symbols = shannon_fano(weights);
        print_result(symbols);
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        std::cerr << "Usage: shannon_fano 0.4 0.3 0.2 0.1\n";
        return 1;
    }

    return 0;
}
