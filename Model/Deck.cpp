#include <iostream>
#include <random>

#include "Card.h"
#include "Deck.h"

using namespace std;

Deck::Deck () {
    for (int i = Card::CLUB; i < Card::SUIT_COUNT; i++) {
        for (int j = Card::ACE; j < Card::RANK_COUNT; j++) {
            CardPtr c = shared_ptr<Card>(
                new Card(static_cast<Card::Suit>(i), static_cast<Card::Rank>(j)));
            cards_.push_back(c);
        }
    }
}

vector<CardPtr> Deck::cards() {
    return cards_;
}

void Deck::shuffle() {
    int seed = 0;
    static mt19937 rng(seed);

    int n = CARD_COUNT;

    while ( n > 1 ) {
        int k = (int) (rng() % n);
        --n;
        CardPtr c = cards_[n];
        cards_[n] = cards_[k];
        cards_[k] = c;
    }
}