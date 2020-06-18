#include "stl.hh"
#include "coin.hh"

using namespace coin;

word_list coin::create_mnemonic(const data_chunk& entropy, const dictionary_t &lexicon)
{
    if ((entropy.size() % coin::mnemonic_seed_multiple) != 0)
        return {};

    using coin::entropy_bit_divisor;

    const size_t entropy_bits = (entropy.size() * byte_bits);
    const size_t check_bits = (entropy_bits / entropy_bit_divisor);
    const size_t total_bits = (entropy_bits + check_bits);
    const size_t word_count = (total_bits / bits_per_word);

    assert((total_bits % bits_per_word) == 0);
    assert((word_count % mnemonic_word_multiple) == 0);

    const auto data = build_chunk({entropy, sha256_hash(entropy)});

    size_t bit = 0;
    word_list words;

    for (size_t word = 0; word < word_count; word++)
    {
        size_t position = 0;
        for (size_t loop = 0; loop < bits_per_word; loop++)
        {
            bit = (word * bits_per_word + loop);
            position <<= 1;

            const auto byte = bit / byte_bits;

            if ((data[byte] & bip39_shift(bit)) > 0)
                position++;
        }

        assert(position < dictionary_size);
        words.push_back(lexicon[position]);
    }

    assert(words.size() == ((bit + 1) / bits_per_word));
    return words;
}
