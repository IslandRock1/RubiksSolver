//
// Created by Øystein Bringsli.
//

#ifndef RUBIKSSOLVER_MOVE_HPP
#define RUBIKSSOLVER_MOVE_HPP


class Move {
public:
    constexpr Move(char m) : move(m) {
        constexpr char moves[18] = {
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'
        };

        short moveIndex = -1;
        for (short i = 0; i < 18; ++i) {
            if (moves[i] == m) {
                moveIndex = i;
                break;
            }
        }

        face = moveIndex / 3;
        rotations = moveIndex % 3 + 1;
    }
    constexpr Move(short face, short rotations): face(face), rotations(rotations) {
        int moveId = face * 3 + rotations - 1;

        constexpr char moves[18] = {
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'
        };

        move = moves[moveId];
    }

    char move;

    short face;
    short rotations;

    bool operator==(const Move &other) const {
        return (move == other.move);
    }
};


#endif //RUBIKSSOLVER_MOVE_HPP
