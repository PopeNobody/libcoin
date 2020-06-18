#include "coin.hh"
#define BITCOIN_ASSERT(x) assert(x)


const std::string base58_chars =
    "123456789"
    "ABCDEFGH"  "JKLMNPQRSTUVWXYZ"
    "abcdefghi" "jkmnopqrstuvwxyz";

bool is_base58(const char ch)
{
    // This works because the base58 characters happen to be in sorted order
    return std::binary_search(base58_chars.begin(), base58_chars.end(), ch);
}
template <typename Data>
auto search_first_nonzero(const Data& data) -> decltype(data.cbegin())
{
    auto first_nonzero = data.cbegin();
    while (first_nonzero != data.end() && *first_nonzero == 0)
        ++first_nonzero;

    return first_nonzero;
}

std::string coin::encode_base58(const data_slice& unencoded)
{
    size_t leading_zeros = count_leading_zeros(unencoded);

    // size = log(256) / log(58), rounded up.
    const size_t number_nonzero = unencoded.size() - leading_zeros;
    const size_t indexes_size = number_nonzero * 138 / 100 + 1;

    // Allocate enough space in big-endian base58 representation.
    data_chunk indexes(indexes_size);

    // Process the bytes.
    for (auto it = unencoded.begin() + leading_zeros;
        it != unencoded.end(); ++it)
    {
        pack_value(indexes, *it);
    }

    // Skip leading zeroes in base58 result.
    auto first_nonzero = search_first_nonzero(indexes);

    // Translate the result into a string.
    std::string encoded;
    const size_t estimated_size = leading_zeros +
        (indexes.end() - first_nonzero);
    encoded.reserve(estimated_size);
    encoded.assign(leading_zeros, '1');

    // Set actual main bytes.
    for (auto it = first_nonzero; it != indexes.end(); ++it)
    {
        const size_t index = *it;
        encoded += base58_chars[index];
    }

    return encoded;
}
size_t coin::count_leading_zeros(const data_slice& unencoded)
{
    // Skip and count leading '1's.
    size_t leading_zeros = 0;
    for (const uint8_t byte: unencoded)
    {
        if (byte != 0)
            break;

        ++leading_zeros;
    }

    return leading_zeros;
}
void coin::pack_value(data_chunk& indexes, size_t carry)
{
    // Apply "b58 = b58 * 256 + ch".
    for (auto it = indexes.rbegin(); it != indexes.rend(); ++it)
    {
        carry += 256 * (*it);
        *it = carry % 58;
        carry /= 58;
    }

    BITCOIN_ASSERT(carry == 0);
}
