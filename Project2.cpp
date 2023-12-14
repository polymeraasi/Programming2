//--------------------------------------------------
//  COMP.CS.110 Ohjelmointi 2: Rakenteet -pohjakoodi
//  Copyright © 2021
//--------------------------------------------------

// Kurssikeskus
//
//Kuvaus:
// Ohjelma kysyy tiedostoa, joka sisältää kurssikeskuksen kursseihin liittyviä
//tietoja: teema, kurssi, paikkakunta sekä osallistujamäärä. Tietojen tallennus
//tapahtuu valittuun tietorakenteeseen, jonka jälkeen käyttäjä pystyy syöttämään
//hakukomentoja siihen. Jos tiedostoa ei voida avata tai tiedostosta ei löydetä
//neljää kenttää, virheilmoitus tulostetaan ja ohjelman suoritus loppuu.
// Hakukamoentoja on kahdeksan erilaista: 'themes', 'courses <paikkakunta>
//<teema>', 'available', 'courses_in_theme <teema>', 'favorite_theme',
//'courses_in_location_ <paikkakunta>', 'cancel <kurssi>' sekä 'quit'. Ohjelman
//suoritus lopetetaan, kun käyttäjä syöttää komennon quit. Virheellisistä
//syötteistä tulostetaan virheilmoitus, mutta ohjelma jatkaa suoritustaan
//tämän jälkeen.
// Komento 'themes' tulostaa tunnetut teemat aakkosjärjestyksessä. Komento
//'courses <paikkakunta> <teema>' tulostaa nimen mukaisessa aakkosjärjestyksessä
//kaikki kurssit ja niiden osallistujamäärät, jotka löytyvät kyseisestä teemasta
//sekä paikkakunnalta. 'available' tulostaa kaikista kursseista ne, jotka eivät
//ole täynnä, eli osallistujamäärä on alle 50. 'courses_in_theme <teema>'
//tulostaa annetun teeman mukaiset kurssit kaikilta paikkakunnilta siinä
//järjestyksessä, kun kurssit on annettu tiedostossa. Komento 'favorite_theme'
//tulostaa suosituimman teeman, jossa osallistujamäärä on suurin. 'cancel <kurssi>'
//komento peruu kurssin kaikilta paikkakunnilta.
// Virhetarkasteluita tehdään syötteille <komento>, <paikkakunta>, <teema>.
//Jos jokin näistä annetaan virheellisessä muodossa tai sitä ei löydy tiedostosta,
//tulostetaan virheilmoitus.
//
// Nimi: Pinja Koivisto

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <map>


const char STRING_FIELD_SEPARATOR = ';';
const int COURSE_ATTENDANCE_FULL = 50;

using namespace std;


// Struct rakenne, johon kurssien tietoja tallennetaan (kurssin nimi,
// sen toteutuksen paikkakunta sekä osallistujamäärä, joka voi saada lukuarvon
// tai merkkijonon 'full').
struct Course {
    string name;
    string location;
    int enrollments;
};


// Jakaa annetun merkkijonon str annetun merkin delimiter erottelemiin osiin
// ja palauttaa osat vektorissa.
// Jos erotinmerkki on lainausmerkkien sisällä, erotinmerkistä ei välitetä,
// eli osaa olla erottelematta lainausmerkkien sisällä olevan tekstin.
std::vector<std::string> split_ignoring_quoted_delim(const std::string& str,
                                                     char delimiter)
{
    std::vector<std::string> result = {""};
    bool inside_quotation = false;
    for(char current_char : str)
    {
        if(current_char == '"')
        {
            inside_quotation = not inside_quotation;
        }
        else if(current_char == delimiter and not inside_quotation)
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    if(result.back() == "")
    {
        result.erase(--result.end());
    }
    return result;
}


// Tarkastelee tiedoston avautumista. Jos tiedosto ei aukea, tulostetaan
// virheilmoitus main-funktiossa.
bool has_file(const std::string& filepath,
              std::vector<std::string>& lines) {

    std::ifstream input_file(filepath);
    if ( ! input_file.is_open()) {
        return false;
    }

    std::string line;
    while ( std::getline(input_file, line)) {
        lines.push_back(line);
    }
    return true;
}


// Tarkastelee annetun tiedoston rakennetta. Jos tietoa on enemmän/vähemmän,
// kuin neljässä kentässä, tulostetaan virheilmoitus mainissa. Käytetään tietoa
// funktiossa analyze_file_lines.
bool is_there_four_fields( const std::vector<std::string>& fields) {

    if ( fields.size() != 4) {
        return false;
    }
    return true;
}


// Tarkastelee annetun tiedoston rakennetta. Jos tiedostossa on jokin kenttä
// tyhjä, tulostetaan virheilmoitus main-funktiossa. Käytetään tietoa funktiossa
// analyze_file_lines.
bool are_fields_empty(const std::vector<std::string>& fields)
   {
    for (const auto& field : fields) {

        if (field.empty()) {
            return false;
        }
    }
    return true;
}


// Funktio lukee tiedoston ja annetaan struct rakenteelle 'Course' oletusarvot.
// Annetun tiedoston sisältämä informaatio luetaan mappiin jossa käytetään
// struct -rakenteen 'Course' sisältämiä alkioita. Map sisältää avaimena
// teeman ja vektorissa Course nimen, paikkakunnan sekä osallistujamäärän.
bool analyze_file_lines(const std::vector<std::string>& lines,
                        std::map<std::string,
                        std::vector<Course>>& courses_by_theme) {

    for (const std::string& line : lines) {

        std::vector<std::string> fields
                = split_ignoring_quoted_delim(line, STRING_FIELD_SEPARATOR);

        if ( ! is_there_four_fields(fields) || ! are_fields_empty(fields)) {
            return false;
        }

        const std::string theme = fields.at(0);
        const std::string& name = fields.at(1);
        const std::string& location = fields.at(2);
        int enrollments = 0;

        if ( fields.at(3) == "full")
        {
            enrollments = COURSE_ATTENDANCE_FULL;
        }

        else {
            //luetaan kenttä numeroiksi, jos kurssi ei ole täynnä.
            std::istringstream enrollments_in(fields.at(3));
            enrollments_in >> enrollments;

            if (! enrollments_in)
            {
                return false;
            }
        }

        Course new_course = Course{name, location, enrollments};
        //jos kurssi on tyhjä, pitää sinne lisätä kenttien tiedot.

        courses_by_theme.try_emplace({theme, {}});
        auto theme_iterator = courses_by_theme.at(theme).begin();

        for ( ; theme_iterator != courses_by_theme.at(theme).end()
              ; ++theme_iterator)
        {
            if ( theme_iterator->name == new_course.name &&
                 theme_iterator->location == new_course.location)
            {
                break;
            }
        }

        if ( theme_iterator != courses_by_theme.at(theme).end())
        {
            courses_by_theme.at(theme).erase(theme_iterator);
        }

        courses_by_theme.at(theme).push_back(new_course);

    }

    return true;
}


// Kun käyttäjä syöttää komennon 'themes', funktio tallentaa annetun tiedoston
// teemat settiin 'themes' ja tulostaa tämän sisällön aakkosjärjestyksessä set-
// rakenteesta.
void themes_command(const std::vector<std::string>& lines)
{
    std::set<std::string> themes = {};

    for (const std::string& line : lines)
    {
        std::vector<std::string> fields
                = split_ignoring_quoted_delim(line, STRING_FIELD_SEPARATOR);

        const std::string theme = fields.at(0);
        themes.insert(theme);
    }

    auto themes_iterator = themes.begin();
    for ( ; themes_iterator != themes.end(); ++themes_iterator)
    {
        std::cout << *themes_iterator << endl;
    }

}

// HUOMIO! Yllä olevat funktiot ovat toteutettu harjoitus-tilaisuudessa, jossa 
// assistentin avustuksella alustettiin projekti. 

// Käyttäjän syötettyä komento 'courses <paikkakunta> <teema>' funktio
// tarkastaa löytyykö annettua paikkakuntaa. Käytetään funktiossa
// courses_command, tulostetaan virheilmoitus jos paikkakuntaa ei löydy.
bool is_location_correct(std::map<std::string,
                         std::vector <Course>> courses_by_theme,
                         std::string i_location)
{
    for (auto& location : courses_by_theme)
    {
        std::vector<Course> course_vector = location.second;
        for ( auto&  find_location : course_vector)
        {
            if (find_location.location == i_location)
            {
                return true;
            }
        }
    }
    return false;
}


// Käyttäjän syötettyä komento 'courses <paikkakunta> <teema> funktio
// tarkastaa löytyykö annettua teemaa. Käytetään funktiossa courses_command,
// tulostetaan virheilmoitus, jos teemaa ei löydy.
bool is_theme_correct(std::map<std::string,
                      std::vector <Course>> courses_by_theme,
                      std::string theme)
{
    if (courses_by_theme.find(theme) == courses_by_theme.end())
    {
        return false;
    }
    return true;
}


// Käyttäjän syötettyä komento 'courses <paikkakunta> <teema> funktio tarkastaa
// löytyykö kyseistä teemaa halutusta paikkakunnasta. Käytetään funktiossa
// courses_command, tulostetaan virheilmoitus jos teemaa ei löydy paikkakunnalta.
bool is_theme_in_location(std::map<std::string,
                           std::vector <Course>> courses_by_theme,
                           std::string i_location, std::string i_theme)
{
    for (Course& courses : courses_by_theme.at(i_theme))
    {
        if (courses.location == i_location)
        {
            return true;
        }
    }
    return false;
}


// Kun käyttäjä syöttää komennon 'courses <paikkakunta> <teema>' funktio
// tulostaa kyseiseltä paikkakunnalta annetusta teemasta kaikki kurssit, jotka
// löytyvät ja niiden osallistujamäärät. Jos kurssi on täynnä, tulostetaan 'full'.
// Funktio myös tarkastaa funktioiden is_location_correct, is_theme_correct sekä
// is_theme_in_location antamat totuusarvot ja tulostaa virheilmoituksen mikäli
// joku näistä on 'false'.
void courses_command(std::map<std::string,
                     std::vector<Course>> courses_by_theme,
                     std::string i_location,
                     std::string i_theme)
{
    if (! is_theme_correct(courses_by_theme, i_theme))
    {
        std::cout << "Error: unknown theme" << endl;
    }
    else if (! is_location_correct(courses_by_theme, i_location)
             or ! is_theme_in_location(courses_by_theme, i_location, i_theme))
    {
        std::cout << "Error: unknown location" << endl;
    }

    else
    {
        std::set<std::pair<std::string, int>> set_from_map_vector = {};
        for (Course& courses : courses_by_theme.at(i_theme))
        {
            if (courses.location == i_location)
            {
                std::pair<std::string, int> name_n_enrollments = {};
                name_n_enrollments.first = courses.name;
                name_n_enrollments.second = courses.enrollments;

                set_from_map_vector.insert(name_n_enrollments);
            }
        }

        for (auto& name_and_enrollments : set_from_map_vector)
        {
            if (name_and_enrollments.second == COURSE_ATTENDANCE_FULL)
            {
                std:: cout << name_and_enrollments.first << " --- "
                << "full" << endl;

            } else {
                std:: cout << name_and_enrollments.first << " --- "
                << name_and_enrollments.second << " enrollments" << endl;
            }
        }
    }
}


// Kun käyttäjä syöttää komennon 'courses_in_location <paikkakunta>, funktio
// tarkastaa onko syötettyä paikkakuntaa olemassa map-rakenteen vektorissa.
// Jos paikkakunta löytyy, tulostetaan kaikki kurssit kaikista teemoista.
void courses_in_location_command(std::map<std::string,
                                 std::vector<Course>> courses_by_theme,
                                 std::string i_location)
{
    if (! is_location_correct(courses_by_theme, i_location))
    {
        std::cout << "Error: unknown location" << endl;
    }

    std::set<std::string> set_of_names = {};
    for (auto& vector_in_map : courses_by_theme)
    {
        std::vector<Course> course_vector = vector_in_map.second;
        for (auto& courses : course_vector)
        {
            if (courses.location == i_location)
            {
                set_of_names.insert(courses.name);
            }
        }
    }

    for (auto& name_of_course : set_of_names)
    {
        std::cout << name_of_course << endl;
    }
}


// Kun käyttäjä syöttää komennon 'available', funktio tulostaa paikkakuntien
// kursseista ne, jotka eivät ole täynnä (osallistujamäärä < 50). Tulostuksen
// järjestys on <teema>, <paikkakunta>, <kurssi> ja kaikki tulostetaan
// aakkorjärjestyksessä.
void available_command(std::map<std::string,
                       std::vector<Course>> courses_by_theme)
{
    auto iterator = courses_by_theme.begin();
    while (iterator != courses_by_theme.end())
    {
        std::map<std::string, std::set<std::string>> map;
        for (Course& courses : iterator->second)
        {
            if (courses.enrollments != COURSE_ATTENDANCE_FULL)
            {
                map.try_emplace({{courses.location}, {}});
                map.at(courses.location).insert(courses.name);
            }
        }

        // iterator osoittaa teemaan
        for (auto& location : map)
        {
            auto course = location.second.begin();
            for (; course != location.second.end() ; ++course)
            {
                std::cout << iterator->first << " : " << location.first <<
                             " : " << *course << endl;
            }
        }
        ++iterator;
    }
}


// Kun käyttäjä syöttää komennon 'courses_in_theme <teema>', ohjelma tulostaa
// kaikilta paikkakunnilta annettuun teemaan kuuluvat kurssit aakkosjärjestyksessä.
void courses_in_theme_command(std::map<std::string,
                         std::vector<Course>> courses_by_theme,
                         std::string i_theme)
{
    if (! is_theme_correct(courses_by_theme, i_theme))
        {
            std::cout << "Error: unknown theme" << endl;
        }

    else
    {
        std::map<std::string, std::vector<std::string>> map_of_location_n_name = {};
        for (Course& courses : courses_by_theme.at(i_theme))
        {
            map_of_location_n_name.try_emplace({courses.location, {}});
            map_of_location_n_name.at(courses.location).push_back(courses.name);
        }

        for (auto& location : map_of_location_n_name)
        {
            for (auto& course_name : location.second)
            {
               std::cout << location.first << " : " << course_name << endl;
            }
        }
    }
}


// Kun käyttäjä syöttää komennon 'favorite_theme', funktio tulostaa
// suosituimman eli osallistujamäärältään suurimman teeman osallistujamäärät
// yhteenlaskettuna sekä kyseisen teeman kurssit. Jos tiedosto on tyhjä,
// tulostetaan 'No enrollments'.
void favorite_theme_command(std::map<std::string,
                            std::vector<Course>> courses_by_theme)
{
    int most_enrollments_by_theme = 0;

    auto map_iterator = courses_by_theme.begin();
    std::set<std::string> themes = {};

    while (map_iterator != courses_by_theme.end())
    {
        int enrollments_by_theme = 0;
        std::map<std::string, int> map_for_enrollments;

        for (Course& courses : map_iterator->second)
        {
            enrollments_by_theme += courses.enrollments;
        }

        map_for_enrollments.insert({map_iterator->first, enrollments_by_theme});

        // Teemojen osallistujamäärien tarkastelu: themes-settiin halutaan se
        // teema, jolla on suurin osallistujamäärä. Voi koostua useista teemoista,
        // jos osallistujamäärät ovat samat.

        auto iter = map_for_enrollments.begin();
        for (; iter != map_for_enrollments.end() ; ++iter)
        {
            if (enrollments_by_theme > most_enrollments_by_theme)
            {
                most_enrollments_by_theme = enrollments_by_theme;
                themes.clear();
                themes.insert(map_iterator->first);
            }
            else if (most_enrollments_by_theme == enrollments_by_theme)
            {
                themes.insert(map_iterator->first);
            }
        }

        ++map_iterator;
    }

    if (most_enrollments_by_theme == 0)
    {
        std::cout << "No enrollments" << endl;
    }

    else
    {
        std::cout << most_enrollments_by_theme <<  " enrollments in themes"
                    << endl;

        for (auto& favorite_theme : themes)
        {
            std::cout << "--- " << favorite_theme << endl;
        }
    }
}


// Tarkastaa syöttäjän 'cancel <kurssi>' -komennossa onko kurssin nimi olemassa
// tiedostossa. Jos ei ole, tulostetaan virheilmoitus. Käytetään funktiossa
// 'cancel_command'.
bool is_course_correct(std::map<std::string,
                       std::vector<Course>>& courses_by_theme,
                       std::string i_course)
{
    for (auto& course : courses_by_theme)
    {
        std::vector<Course> course_vector = course.second;
        for ( auto&  find_course : course_vector)
        {
            if (find_course.name == i_course)
            {
                return true;
            }
        }
    }
    return false;
}


// Kun käyttäjä syöttää komennon 'cancel <kurssi>', funktio poistaa kyseisen
// kurssin kaikilta paikkakunnilta, joissa se esiintyy. Alussa suoritetaan
// tarkastus, onko poistettavaa kurssia olemassa tiedostossa.
void cancel_command(std::map<std::string,
                    std::vector<Course>>& courses_by_theme,
                    std::string i_course)
{
    if (! is_course_correct(courses_by_theme, i_course))
    {
        std::cout << "Error: unknown course" << endl;

    } else {

    for (auto& map : courses_by_theme)
    {
        std::vector<Course>::iterator vector_iterator =  map.second.begin();
        while (vector_iterator != map.second.end())
        {
            if (vector_iterator->name == i_course)
            {
                vector_iterator = map.second.erase(vector_iterator);
            }
            else {
                ++vector_iterator;
            }
        }  
    }

    std::cout << i_course << " cancelled in all locations" << endl;
    }
}


int main()
{
    std::string filename;
    std::cout << "Input file: ";
    std::getline(std::cin, filename);
    std::vector<std::string> input_file_lines;

    //tiedoston avautumisen tarkastelu
    if (has_file(filename, input_file_lines) == false ) {
        std::cout << "Error: the input file cannot be opened" << endl;
        return EXIT_FAILURE;
    }

    std::map<std::string, std::vector<Course>> courses_by_theme;

    //tiedoston sisältämien kenttien tarkastelu:
    if (analyze_file_lines(input_file_lines, courses_by_theme) == false )
    {
        std::cout << "Error: empty field" << endl;
        return EXIT_FAILURE;
    }

    //jos tiedosto on määritelmän mukainen, ohjelman suoritus alkaa:
    while (true)
    {
        std::string line;
        std::cout << "> ";
        getline(std::cin, line);
        std::vector<std::string> fields_of_input =
                split_ignoring_quoted_delim(line, ' ');

        std::string user_command = fields_of_input.at(0);


        if ( user_command == "themes" )
        {
            themes_command(input_file_lines);
        }


        else if ( user_command == "courses")
        {
            if ( fields_of_input.size() != 3)
            {
                std::cout << "Error: error in command courses" << std::endl;
            }
            else
            {
                std::string location = fields_of_input.at(1);
                std::string theme = fields_of_input.at(2);
                courses_command(courses_by_theme, location, theme);
            }
        }

        else if (user_command == "available")
        {
            available_command(courses_by_theme);
        }


        else if (user_command == "courses_in_theme")
        {
            if (fields_of_input.size() != 2)
            {
                std::cout << "Error: error in command courses" << std::endl;
            }
            std::string location = fields_of_input.at(1);
            courses_in_theme_command(courses_by_theme, location);
        }


        else if (user_command == "courses_in_location")
        {
            if (fields_of_input.size() != 2)
            {
                std::cout << "Error: error in command courses" << std::endl;
            }

            std::string theme = fields_of_input.at(1);
            courses_in_location_command(courses_by_theme, theme);
        }


        else if (user_command == "favorite_theme")
        {
            favorite_theme_command(courses_by_theme);
        }


        else if (user_command == "cancel")
        {
            if (fields_of_input.size() != 2)
            {
                std::cout << "Error: error in command courses" << std::endl;
            }

            std::string course = fields_of_input.at(1);
            cancel_command(courses_by_theme, course);
        }


        else if (user_command == "quit")
        {
            return EXIT_SUCCESS;
        }


        else {
            std::cout << "Error: Unknown command: " << user_command << std::endl;
        }

    }

    return EXIT_SUCCESS;
}

