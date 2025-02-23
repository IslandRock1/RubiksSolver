//
// Created by Øystein Bringsli.
//

#include <iostream>
#include "RubiksCube.hpp"
#include "Move.hpp"

Move::Move(char m): move(m) {
    constexpr char moves[18] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
            'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'
    };

    int moveIndex = -1;
    for (int i = 0; i < 18; ++i) {
        if (moves[i] == m) {
            moveIndex = i;
            break;
        }
    }

    if (moveIndex == -1) {
        std::string error = "Invalid char '";
        error.push_back(m);
        error = error + "' for move generation";
        throw std::runtime_error(error);
    }

    face = moveIndex / 3;
    rotations = moveIndex % 3 + 1;
}

void Move::updateFaceRot() {
    constexpr char moves[18] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
            'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'
    };

    int moveIndex = -1;
    for (int i = 0; i < 18; ++i) {
        if (moves[i] == move) {
            moveIndex = i;
            break;
        }
    }

    if (moveIndex == -1) {
        std::string error = "Invalid char '";
        error.push_back(move);
        error = error + "' for move generation";
        throw std::runtime_error(error);
    }

    face = moveIndex / 3;
    rotations = moveIndex % 3 + 1;
}

Move::Move(int face, int rotations): face(face), rotations(rotations) {
    int moveId = face * 3 + rotations - 1;

    constexpr char moves[18] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
            'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'
    };

    move = moves[moveId];
}

void Move::updateChar() {
    int moveId = face * 3 + rotations - 1;

    constexpr char moves[18] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
            'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'
    };

    move = moves[moveId];
}

void insertMove(std::vector<Move> &moves, Move &m) {
    if (moves.empty()) {
        moves.emplace_back(m);
        return;
    }

    if (moves.back().face == m.face) {
        moves.back().rotations = (moves.back().rotations + m.rotations) % 4;
        if (moves.back().rotations == 0) {
            moves.pop_back();
        } else {
            moves.back().updateChar();
        }
    } else if ((moves.size() > 1) and (moves.back().face == RubiksConst::oppositeFaceAll[m.face]) and (moves.at(moves.size() - 2).face == m.face)) {
        auto ix = moves.size() - 2;
        moves.at(ix).rotations = (moves.at(ix).rotations + m.rotations) % 4;
        moves.at(ix).updateChar();
    } else {
        moves.emplace_back(m);
    }
}

std::vector<Move> Move::combineMovesWithLookupMoves(const std::vector<Move>& moves, std::vector<Move> lookupMoves, bool reverse) {
    std::vector<Move> outMoves;

    for (auto m : moves) {
        outMoves.emplace_back(m);
    }

    if (reverse) {
        std::reverse(lookupMoves.begin(), lookupMoves.end());
    }

    for (auto m : lookupMoves) {
        outMoves.emplace_back(m.face, 4 - m.rotations);
    }

    return outMoves;
}


std::vector<Move> Move::combineMoves(std::vector<Move> &firstMoves, std::vector<Move> &secondMoves) {
    std::vector<Move> outMoves;

    for (auto &m : firstMoves) {
        insertMove(outMoves, m);
    }

    for (auto &m : secondMoves) {
        insertMove(outMoves, m);
    }

    return outMoves;
}

std::vector<Move> Move::combineMoves(std::vector<std::vector<Move>> &moves) {
    std::vector<Move> outMoves;

    for (auto &moveVector : moves) {
        for (auto &move : moveVector) {
            insertMove(outMoves, move);
        }
    }

    return outMoves;
}

std::vector<Move> Move::combineMoves(std::vector<Move> &moves) {
    std::vector<Move> out;

    for (auto &move : moves) {
        insertMove(out, move);
    }

    return out;
}

std::vector<char> Move::convertVectorMoveToChar(const std::vector<Move> &moves) {
    std::vector<char> out;
    for (auto &m : moves) {
        out.push_back(m.move);
    }

    return out;
}

std::vector<Move> Move::convertVectorCharToMove(const std::vector<char> &moves) {
    std::vector<Move> out;
    for (auto &m : moves) {
        out.emplace_back(m);
    }

    return out;
}

void Move::printMoves(std::vector<Move> &moves) {
    std::cout << "Moves: ";

    for (auto m : moves) {
        switch (m.face) {
        case 0:
        {
            std::cout << "U";
        } break;

        case 1:
        {
            std::cout << "L";
        } break;

        case 2:
        {
            std::cout << "F";
        } break;

        case 3:
        {
            std::cout << "B";
        } break;

        case 4:
        {
            std::cout << "R";
        } break;

        case 5:
        {
            std::cout << "D";
        } break;
        }

        std::cout << m.rotations << " ";
    }

    std::cout << "\n";
}



