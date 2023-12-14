#include <iostream>
#include <vector>
#include <string>


// Tehtävä: Ristinolla-peli
// Kuvaus: Ohjelma toteuttaa neliön mallisen pelialustan, jota pystyy pelin edetessä tarvittaessa laajentamaan. Alussa aloitetaan 3x3 kokoisella alustalla.
//         Alustalla on kolmenlaisia merkkejä: X, O ja tyhjä (.). Pelin voittaa se, joka saa ensimmäiseksi muodostettua pelimerkeistään kokonaisen rivin, diagonaalin tai sarakkeen.
//         Jokaisen kierroksen alussa vuorossa olevalta kysytään koordinaatteja (x, y), jonne haluaa sijoittaa pelimerkkinsä.
//         Merkki sijoitetaan halutulle pelialustalle annettuihin koordinaatteihin, ja jos ei vuorolla tule voittoa, siirtyy vuoro seuraavalle (merkille).
//         Pelin on mahdollista päättyä tilanteeseen, jossa pelialusta on täynnä merkkejä, mutta kumpikaan merkki ei ole voittanut.
//         Pelialusta laajenee pelin edetessä, jos syötetään yhtä lukua suurempi koordinaatti, kun ruudulla näkyy. Ohjelma tarkistaa, että ruudukko täyttyy korkeintaan yhden ruudun kerrallaan per kierros.
//         Lopuksi ohjelma ilmoittaa, kumpi merkeistä on voittanut, jos voittaja selviää.

// Nimi: Pinja Koivisto

//alussa pelilaudan koko on 3x3
int STARTING_GRID_SIZE = 3;


void init_board(std::vector<std::vector<char>>& board, size_t width_length)
// Alustaa tulostuksen, joka tulostaa ensimmäisen pelialustan (3x3).
{
    for ( size_t colums = 0; colums < width_length; ++colums) {
        std::vector<char> column_vector(width_length, '.');
        board.push_back(column_vector);
    }
}


void print_board(std::vector<std::vector<char>>& board, std::ostream&stream )
// Tulostaa pelialustan siten, että ensimmäinen rivi ja sarake ovat numeroita.
{
    size_t size = board.size();
    stream << " ";
    for (size_t x = 0; x < size; ++x) {
        stream << (x + 1) % 10 << ' ';
    }
    stream << std::endl;
    for (size_t y = 0; y < size; ++ y) {
        stream << (y + 1) % 10 << ' ';
        for (size_t x = 0; x < size; ++x) {
            std::cout<<board.at(y).at(x);
            stream << " ";
      }
        stream << std::endl;
    }
    std::cout << std::endl;
}

// HUOMIO: Yllä olevat funktiot on tehty harjoitustilaisuudessa, kurssiassistentin avustuksella.

bool is_number_int(std::string input)
// Tarkistaa onko annetut koordinaatit lukuja, jos ei, tulostaa virheilmotuksen
// funktiossa read_command
{
    for (char cin : input) {
        if ( std::isdigit(cin) == 0) {
            return false;
        }
    }
    return true;
}


void board_broaden_size(std::vector<std::vector<char>>& board,
                        int x, int y)
// Funktio laajentaa pelialustan annettujen koordinaattien mukaan joko oikealle
// alakulmaan tai vasemalle yläkulmaan
{
    size_t board_length = board.size() +1;
    int board_length_as_int = static_cast<int>(board_length);

    // oikea ala
    if ( x == board_length_as_int or y == board_length_as_int) {
        for ( size_t i = 0; i < board_length-1; ++i) {
            board.at(i).push_back('.');
        }

        std::vector<char>row = {};
        for ( size_t j = 0; j < board_length; ++j) {
            row.push_back('.');

        }
        board.push_back(row);

    }
    // vasen ylä
    if ( x == 0 or y == 0) {
        std::vector<std::vector<char>>new_board = {};
        for ( size_t columns = 0; columns < board_length; ++columns) {
            std::vector<char>new_column_vector(board_length, '.');
            new_board.push_back(new_column_vector);
        }
        for ( size_t i = 0; i < board_length-1;++i) {
            for ( size_t j = 0; j < board_length-1;++j) {
                new_board.at(i+1).at(j+1) = board.at(i).at(j);

            }
        }
        board = new_board;

    }
 }


bool is_size_allowed(std::vector<std::vector<char>>& board,
                     int x, int y)
// funktio katsoo, onko annetut koordinaatit sallittuja pelialustan koon suhteen
{
    size_t board_length = board.size() +1;
    bool is_coordinates_allowed = true;
    int board_length_as_int = static_cast<int>(board_length);

    // jos x tai y -koordinaatti menee kahden yksikön verran yli laudasta
    if (x > board_length_as_int or y > board_length_as_int) {
        is_coordinates_allowed = false;
    }
    // jos x tai y -koordinaatti menee alle nollan
    else if ( x < 0 or y < 0) {
        is_coordinates_allowed = false;
    }
    // jos x menee alle nollan ja samalla y ylittää laudan
    else if ( x == 0 and y == board_length_as_int) {
        is_coordinates_allowed = false;
    }
    // jos x menee yli laudan ja y alle nollan
    else if ( x == board_length_as_int and y == 0) {
        is_coordinates_allowed = false;

    }else{
        is_coordinates_allowed = true;
    }

    return is_coordinates_allowed;
}


bool has_char(std::vector<std::vector<char>>& board, int x, int y)
//  funktio tarkastaa onko pelialustalla annettujen koordinaattien kohdalla jo merkkiä
{
    bool has_char_in_coords = false;

    // Jos lauta laajenee vasemmalle:
    if (x == 0 or y == 0) {
        x += 1;
        y += 1;
    }
    if (board.at(y-1).at(x-1) != '.') {
          has_char_in_coords = true;
     }else{
        has_char_in_coords = false;
    }
    return has_char_in_coords;
}


bool is_full(std::vector<std::vector<char>>& board)
// funktio katsoo, onko pelilauta täynnä merkkejä ilman, että kukaan on voittanut
{
    bool is_board_full = true;
    size_t length_of_board = board.size();
    for( size_t i = 0; i< length_of_board;++i) {
        for( size_t j = 0; j< length_of_board;++j) {
            if( board.at(i).at(j) == '.') {
                is_board_full = false;
            }
        }
    }
    return is_board_full;
}


std::vector<int> read_command(char char_in_turn,
                                       std::vector<std::vector<char>>& board)
// Funktio lukee käyttäjältä syötteen ja jos annettu syöte on mahdollinen
// pelin jatkumisen kannalta, muuttaa se annetun merkkijonon kokonaisluvuiksi
// ja tallentaa koordinaatit given_coordinates -nimiseen vektoriin.
// Jos annetut koordinaatit menevät yli laudan, ottaa funktio yhteyttä
// board_broaden_size - funktioon, jossa laajennetaan pelialustaa.
{
    std::vector<int> given_coordinates = {};

    bool input = false;
    size_t board_length = board.size();
    int board_length_as_int = static_cast<int>(board_length);

    while (input == false) {
        std::string x_coords;
        std::string y_coords;
        std::cout << "For " << char_in_turn << ", enter coordinates: x y> ";
        std::cin>> x_coords;

        //jos pelaaja syöttää "q", peli päättyy. Tarkastellaan main()-funktiossa
        if ( x_coords == "q") {
            std::cout << "Why on earth you are giving up the game?" <<std::endl;
            given_coordinates.push_back(-1);
            given_coordinates.push_back(-1);
            input = true;

        //jos syöte ei ole "q" tarkastellaan annettuja koordinaatteja seuraavasti:

        } else {
        std::cin>>y_coords;

        //jos syöte ei ole numeroita
         if (is_number_int(x_coords) == false or is_number_int(y_coords) == false) {
            std::cout << "Coordinate outside the board" << std::endl;

         //jos koordinaatit eivät ole sallituissa rajoissa
         } else if (is_size_allowed(board, stoi(x_coords), stoi(y_coords)) == false) {
            std::cout << "Coordinate outside the board" << std::endl;

        //Jos koordinaatit sallittuja, katsotaan taytyykö lautaa laajentaa vai ei.
        //Jos lautaa joudutaan laajentamaan, ottaa yhteyttä board_broaden size-
        //funktioon. Samalla katsotaan, onko annettujen koordinaattien kohdalla jo
        //laitettu merkki.
        } else if (is_size_allowed(board, stoi(x_coords), stoi(y_coords)) == true) {
            int x = 0;
            int y = 0;
            x = stoi(x_coords);
            y = stoi(y_coords);

            if (x == 0 or y == 0 ) {
                board_broaden_size(board, stoi(x_coords), stoi(y_coords));
            }
            if ( x > board_length_as_int or y > board_length_as_int){
                board_broaden_size(board, stoi(x_coords), stoi(y_coords));

            }if (has_char(board, stoi(x_coords), stoi(y_coords)) == true ) {
                       std::cout << "The given place is already occupied"<< std::endl;
            }else{
                given_coordinates.push_back(x);
                given_coordinates.push_back(y);
                input = true;
            }
        }
    }
    }
    // Palauttaa vektorin, joka sisältää annetut koordinaatit
    return given_coordinates;
}


void add_mark(std::vector<int > given_coordinates,
              std::vector<std::vector<char>>& board, char char_in_turn)
// Funktio tulostaa vuorossa olevan pelaajan merkin annetun koordinaatin paikalle.
{
    int x = given_coordinates.front();
    int y = given_coordinates.back();

    // Jos funktio laajenee vasemmalle:
    if ( x == 0 or y == 0) {
        x += 1;
        y += 1;
    }

    board.at(y-1).at(x-1) = char_in_turn;

}


bool has_won_horizontally(std::vector<std::vector<char>>& board,
             char char_in_turn)
// Funktio tarkistaa voittajan vaakasuunnassa.
{
    size_t length_of_board = board.size();
    bool won_horizontally = false;

    for (size_t i = 0; i < length_of_board; ++i) {
        size_t how_many_chars = 0;
        for ( size_t j = 0; j < length_of_board; ++j) {
            if ( board.at(i).at(j) == char_in_turn) {
                ++how_many_chars;
            }
            if ( how_many_chars == length_of_board) {
                won_horizontally = true;
            }
        }
    }
    return won_horizontally;
}


bool has_won_vertically(std::vector<std::vector<char>>& board,
                          char char_in_turn)
// Funktio tarkastaa voiton pystysuunnassa.
{
    size_t length_of_board = board.size();
    bool won_vertically = false;

    for ( size_t j = 0; j < length_of_board; ++j) {
        size_t how_many_chars = 0;
        for ( size_t i = 0; i< length_of_board; ++i) {
            if ( board.at(i).at(j) == char_in_turn) {
                ++ how_many_chars;
            }
            if ( how_many_chars == length_of_board) {
                won_vertically = true;
            }
        }
    }
    return won_vertically;
}


bool has_won_diagonally_upper_corner(std::vector<std::vector<char>>& board,
                   char char_in_turn)
// Funktio tarkastaa voiton diagonaalisuunnassa vasemmasta yläkulmasta
// Oikealle alakulmaan.
{
    size_t length_of_board = board.size();
    bool won_diagonally_upper = false;
    size_t how_many_chars = 0;

    for ( size_t j = 0; j < length_of_board; ++j) {

        if ( board.at(j).at(j) == char_in_turn) {

                ++ how_many_chars;
        }
            if ( how_many_chars == length_of_board) {
                won_diagonally_upper = true;
            }
        }
    return won_diagonally_upper;
}


bool has_won_diagonally_lower_corner(std::vector<std::vector<char>>& board,
                                     char char_in_turn)
// Funktio tarkastaa voiton diagonaalisuunnasta vasemmasta alakulmasta
// Oikealle yläkulmaan.
{
    size_t length_of_board = board.size();
    bool won_diagonally_lower = false;
    size_t how_many_chars = 0;

    for ( size_t j = 0; j < length_of_board; ++j) {

        if ( board.at(length_of_board-j-1).at(j) == char_in_turn) {

                ++ how_many_chars;
        }
            if ( how_many_chars == length_of_board) {
                won_diagonally_lower = true;
            }
        }
    return won_diagonally_lower;
}


int main()
{
    std::vector<std::vector<char>> board = {};
    std::vector<int > given_coordinates;
    init_board(board, STARTING_GRID_SIZE);
    std::cout << "Start game:" << std::endl;
    print_board(board, std::cout);

    unsigned int integer = 1;

    while (true) {
        char char_in_turn = ' ';
        if ( integer % 2 == 0) {
             char_in_turn = '0';
        } else if(integer % 2 == 1) {
             char_in_turn = 'X';
        }

        given_coordinates = read_command( char_in_turn, board);

        //Jos käyttäjä syöttää "q", peli päättyy.
        if ( given_coordinates.at(0) == -1 and given_coordinates.at(1) == -1) {
            return EXIT_SUCCESS;

        // Muuten peli jatkuu:
        }else {
        integer += 1;

        add_mark(given_coordinates, board, char_in_turn);
        print_board(board, std::cout);

        if (has_won_horizontally(board, char_in_turn) == true){
            if (char_in_turn == 'X') {
                std::cout << "Cross won horizontally" <<std::endl;
                std::cout << "Game over!" <<std::endl;
                return 0;
            } else {
                std::cout << "Nougth won horizontally" << std::endl;
                std::cout << "Game over!"<<std::endl;
                return 0;
        }
        }
        if(has_won_vertically(board, char_in_turn) == true){

            if(char_in_turn == 'X') {
                std::cout << "Cross won vertically" <<std::endl;
                std::cout << "Game over!"<<std::endl;
                return 0;
            } else {
                std::cout << "Nought won vertically" <<std::endl;
                std::cout << "Game over!"<<std::endl;;
                return 0;
        }
        }
        if(has_won_diagonally_upper_corner(board, char_in_turn)==true){
            if (char_in_turn == 'X'){
                std::cout << "Cross won diagonally" <<std::endl;
                std::cout<< "Game over!"<<std::endl;
                return 0;
            }else{
                std::cout << "Nought won diagonally"<<std::endl;
                std::cout << "Game over!"<<std::endl;
                return 0;
            }
        }
        if( has_won_diagonally_lower_corner(board, char_in_turn) == true) {
            if (char_in_turn == 'X'){
                std::cout << "Cross won diagonally" <<std::endl;
                std::cout<< "Game over!"<<std::endl;
                return 0;
            }else{
                std::cout << "Nought won diagonally"<<std::endl;
                std::cout << "Game over!"<<std::endl;
                return 0;
            }
        }
        if(is_full(board) == true) {
            std::cout<< "No empty places"<<std::endl;
            std::cout<< "Game over!"<<std::endl;
            return 0;
        }
    }
    }
    return EXIT_SUCCESS;
}
