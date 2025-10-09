struct coordinates{
    private:
        int x;
        int y;

    public:
        coordinates(): x(0), y(0){}
        coordinates(int _x, int _y): x(_x), y(_y){}

        int getX() { return x; }; //получение координаты X
        int getY() { return y; }; //получение координаты Y
};




//типы фигур
enum class pieceType{
    king, //король
    queen, //ферзь
    rook, //ладья
    bishop, //слон
    knight, //конь
    pawn //пешка
};

//цвет фигур
enum class color{
    black, //черный
    white //белый
};