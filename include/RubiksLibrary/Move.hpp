
#include <stdexcept>
#include <vector>

#ifndef RUBIKSSOLVER_MOVE_HPP
#define RUBIKSSOLVER_MOVE_HPP


class Move {
public:
    explicit Move(char m);
    Move(int face, int rotations);

    void updateChar();
    void updateFaceRot();

    char move;

    int face;
    int rotations;

    bool operator==(const Move &other) const {
        return (move == other.move);
    }

    static std::vector<Move> combineMovesWithLookupMoves(const std::vector<Move>& moves, std::vector<Move> lookupMoves, bool reverse = true);
    static std::vector<Move> combineMoves(std::vector<Move> &firstMoves, std::vector<Move> &secondMoves);
    static std::vector<Move> combineMoves(std::vector<std::vector<Move>> &moves);
    static std::vector<Move> combineMoves(std::vector<Move> &moves);

    static std::vector<char> convertVectorMoveToChar(const std::vector<Move> &moves);
    static std::vector<Move> convertVectorCharToMove(const std::vector<char> &moves);

    static void printMoves(std::vector<Move> &moves);
    static void printMoves(const std::vector<char> &moves, const std::string& end = "\n");
};


#endif //RUBIKSSOLVER_MOVE_HPP
