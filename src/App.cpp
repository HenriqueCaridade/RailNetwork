
#include <exception>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <filesystem>

#include "App.h"

using namespace std;


static void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

App::App() = default;


void App::initializeData() {
    railMan.initializeData(datasetPath);
}


string App::getDoubleString(const string& question, const string& invalidMessage, bool (*isValid)(double)) {
    auto isDouble = [](const string& str){
        char* end = nullptr;
        double val = strtod(str.c_str(), &end);
        return end != str.c_str() && *end == '\0' && val != HUGE_VAL;
    };
    while (true){
        cout << vertical << ' ' << question << flush;
        string aux;
        cin >> aux;
        cout << getBottomLine() << endl;
        if(aux == "x" || (isDouble(aux) && isValid(stod(aux)))) return aux;
        cout << vertical << ' ' << invalidMessage << endl;
    }
}


template<class T>
T App::getInput(const string& question, const string& invalidMessage, const unordered_set<T>& validOptions) {
    while (true){
        cout << vertical << ' ' << question << flush;
        T aux;
        cin >> aux;
        cout << getBottomLine() << endl;
        if(validOptions.empty() || (validOptions.find(aux) != validOptions.end())) return aux;
        cout << vertical << ' ' << invalidMessage << endl;
    }
}
string App::getLine(const string& question, const string& invalidMessage, const unordered_set<string>& validOptions) {
    while (true){
        cout << vertical << ' ' << question << flush;
        string aux;
        getline(cin, aux);
        cout << getBottomLine() << endl;
        if(validOptions.empty() || (validOptions.find(aux) != validOptions.end())) return aux;
        cout << vertical << ' ' << invalidMessage << endl;
    }
}

string App::fillLine(const string& str) {
    return str + string(titleSize - str.size() - 1, ' ') + vertical;
}

string App::getTitle(const string& title){
    int aux = titleSize - (int) title.size();
    if(aux < 4) throw std::exception();
    aux = (aux - 2) / 2;
    string out = corner + string(aux, horizontal) + ' ' + title + ' ';
    out += string(titleSize - out.size() - 1, horizontal) + corner;
    return out;
}

string App::getMenuLine(const string& str){
    return fillLine(vertical + (' ' + str));
}

string App::getBottomLine(){
    return (corner + string(titleSize - 2, horizontal) + corner);
}

string App::getOption(const vector<string>& options, int index){
    if(index >= options.size()) return vertical + string(titleSize - 2, ' ') + vertical;
    return getMenuLine(options[index]);
}

void App::drawMenu(const string& title, const vector<string>& options){
    cout << "\n" << getTitle(title) << string(spaceBetween, ' ') << '\n';
    for(int i = 0; i < options.size(); i++)
        cout << getOption(options, i) << string(spaceBetween, ' ') << '\n';
    cout << getBottomLine() << string(spaceBetween, ' ') << endl;
}

template <typename Lambda>
void App::runMenu(const string &title, const vector<pair<char, string>> &options, Lambda f, bool clearFirst, bool clearLast) {
    if (clearFirst) clear_screen();
    bool running = true;
    vector<string> optionsText;
    unordered_set<char> optionsChar;
    for(const auto& p : options){
        optionsChar.insert(p.first);
        optionsText.push_back(p.first + string(" - ") + p.second);
    }
    while(running){
        drawMenu(title, optionsText);
        char choice = getInput("Choice:", "Invalid Choice. Try Again.", optionsChar);
        running = f(choice);
    }
    if (clearLast) clear_screen();
}


void App::start(){
    dataSelectionMenu();
    /*
    // Santar├®m
    const string st1 = "Lisboa Oriente";
    const string st2 = "Aveiro - Vouga";
    unsigned maxF1, maxF2;
    maxF1 = railMan.maxFlow(st1, st2);
    maxF2 = railMan.maxFlow(st2, st1);
    cout << maxF1 << ' ' << maxF2 << endl;

    system("PAUSE");
    */
     mainMenu();
}

void App::mainMenu() {
    runMenu("Rail Manager", {
            {'1', "Basic Service Metrics"},
            {'2', "Operation Cost Optimization"},
            {'3', "Reliability and Sensitivity to Line Failures"},
            {'d', "Data Selection"},
            {'x', "Exit App"}
    }, [this](char choice) -> bool {
        switch(choice){
            case '1': basicServiceMenu(); break;
            case '2': costMenu(); break;
            case '3': reliabilityMenu(); break;
            case 'd': dataSelectionMenu(); return true;
            case 'x': return false;
        }
        clear_screen();
        return true;
    }, false);
}


// ================== //
// BASIC SERVICE MENU //
// ================== //

void App::basicServiceMenu() {
    runMenu("Basic Service Metrics", {
            {'1', "Max Number of Trains from A to B."},
            {'2', "Important Stations Pairs"},
            {'3', "Larger Budget Demanding Places"},
            {'4', "Max Number of Trains that Arrive at a Station"},
            {'x', "Back"}
    }, [this](char choice) -> bool {
        switch(choice){
            case '1': maxFlowOption(); break;
            case '2': importantStationsOption(); break;
            case '3': largerBudgetPlacesOption(); break;
            case '4': maxFlowStationOption(); break;
            case 'x': return false;
        }
        return true;
    });
}

void App::maxFlowOption() {
    string origin, destination;
    unordered_set<string> stationNames;
    for (const auto& [name, _] : railMan.stations)
        stationNames.insert(name);
    stationNames.insert("x"); // Cancel Option
    cin.ignore(); // Ignore \n char from previous choice.
    origin = getLine("Origin Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
    if (origin == "x") return;
    destination = getLine("Destination Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
    if (destination == "x") return;
    cout << " - Max Flow between " << origin << " and " << destination << " -" << endl;
    cout << "Max Flow: " << railMan.maxFlow(origin, destination) << endl;
}

void App::importantStationsOption() {
    cout << " - Important Stations Pairs -" << endl;
    auto [stations, maxFlow] = railMan.importantStations();
    cout << "Max Flow: " << maxFlow << '\n';
    for (const auto& [stationA, stationB] : stations)
        cout << stationA << " - " << stationB << '\n';
    cout << flush;
}

void App::largerBudgetPlacesOption() {
    string input = getDoubleString("Top k Municipalities (> 0) (x to Cancel):", "Invalid Number. Try Again.", [](double x) -> bool { return x > 0; });
    if (input == "x") return;
    int munK = ceil(stod(input));
    input = getDoubleString("Top k Districts (> 0) (x to Cancel):", "Invalid Number. Try Again.", [](double x) -> bool { return x > 0; });
    if (input == "x") return;
    int disK = ceil(stod(input));
    cout << " - Top " << munK << " Municipalities -" << endl;
    for (const auto& [municipality, maxF] : railMan.topMunicipalities(munK))
        cout << municipality << " - " << maxF << '\n';
    cout << "\n - Top " << disK << " Districts -" << endl;
    for (const auto& [district, maxF] : railMan.topDistricts(disK))
        cout << district << " - " << maxF << '\n';
    cout << flush;
}

void App::maxFlowStationOption() {
    string station;
    unordered_set<string> stationNames;
    for (const auto& [name, _] : railMan.stations)
        stationNames.insert(name);
    stationNames.insert("x"); // Cancel Option
    cin.ignore(); // Ignore \n char from previous choice.
    station = getLine("Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
    if (station == "x") return;
    cout << " - Max Flow of " << station << " -" << endl;
    cout << "Max Flow: " << railMan.maxFlowStation(station) << endl;
}


// ========= //
// COST MENU //
// ========= //

void App::costMenu() {
    runMenu("Operation Cost Optimization", {
            {'1', "Max Amount of Trains that Can Travel with Minimum Cost"},
            {'x', "Back"}
    }, [this](char choice) -> bool {
        switch(choice){
            case '1': maxFlowMinCostOption(); break;
            case 'x': return false;
        }
        return true;
    });
}

void App::maxFlowMinCostOption() {
    string origin, destination;
    unordered_set<string> stationNames;
    for (const auto& [name, _] : railMan.stations)
        stationNames.insert(name);
    stationNames.insert("x"); // Cancel Option
    cin.ignore(); // Ignore \n char from previous choice.
    origin = getLine("Origin Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
    if (origin == "x") return;
    destination = getLine("Destination Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
    if (destination == "x") return;
    cout << " - Max Flow with the Minimum Cost between " << origin << " and " << destination << " -" << endl;
    cout << "Max Flow: " << railMan.maxFlowMinCost(origin, destination) << endl;
}


// ================ //
// RELIABILITY MENU //
// ================ //

void App::reliabilityMenu() {
    runMenu("Reliability and Sensitivity to Line Failures", {
            {'1', "Max Flow for Reduced Connectivity"},
            {'2', "Most Sensitive Stations"},
            {'3', "Set Line and Station Failures"},
            {'x', "Back"}
    }, [this](char choice) -> bool {
        switch(choice){
            case '1': maxFlowReducedOption(); break;
            case '2': mostSensitiveStationsOption(); break;
            case '3': reducedSettingsMenu(); break;
            case 'x': return false;
        }
        return true;
    });
}

void App::maxFlowReducedOption() {
    string origin, destination;
    unordered_set<string> stationNames;
    for (const auto& [name, _] : railMan.stations)
        stationNames.insert(name);
    stationNames.insert("x"); // Cancel Option
    cin.ignore(); // Ignore \n char from previous choice.
    origin = getLine("Origin Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
    if (origin == "x") return;
    destination = getLine("Destination Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
    if (destination == "x") return;
    cout << " - Max Flow of the Reduced Network between " << origin << " and " << destination << " -" << endl;
    cout << "Max Flow: " << railMan.maxFlowReduced(origin, destination, segmentsToDeactivate, stationsToDeactivate) << endl;
}

void App::mostSensitiveStationsOption() {
    string input = getDoubleString("Top k (> 0) (x to Cancel):", "Invalid Number. Try Again.", [](double x) -> bool { return x > 0; });
    if (input == "x") return;
    int k = ceil(stod(input));
    cout << " - Top " << k << " Most Sensitive Stations -" << endl;
    for (const auto& station : railMan.topAffectedStations(k,segmentsToDeactivate,stationsToDeactivate))
        cout << station << '\n';
    cout << flush;
}

void App::reducedSettingsMenu() {
    runMenu("Line and Station Failures", {
            {'1', "Toggle Affected Segments"},
            {'2', "Toggle Affected Stations"},
            {'3', "Clear Affected Segments"},
            {'4', "Clear Affected Stations"},
            {'c', "Clear All"},
            {'s', "See Current Settings"},
            {'x', "Back"}
    }, [this](char choice) -> bool {
        switch(choice){
            case '1': { // Toggle Affected Segments
                string stationA, stationB;
                unordered_set<string> stationNames;
                for (const auto& [name, _] : railMan.stations)
                    stationNames.insert(name);
                stationNames.insert("x"); // Cancel Option
                cin.ignore(); // Ignore \n char from previous choice.
                while (true) {
                    stationA = getLine("Origin Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
                    if (stationA == "x") return true;
                    stationB = getLine("Destination Station (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
                    if (stationB == "x") return true;
                    if (railMan.segmentExists(stationA, stationB)) break;
                    cout << vertical << ' ' << stationA << " - " << stationB << " Segment doesn't Exist.\n" << getBottomLine() << endl;
                }
                for (auto it = segmentsToDeactivate.begin(); it != segmentsToDeactivate.end(); it++)
                    if (it->first == stationA && it->second == stationB){
                        segmentsToDeactivate.erase(it);
                        cout << "Reactivated Segment " << stationA << " - " << stationB << endl;
                        goto dont_add_segment;
                    }
                segmentsToDeactivate.emplace_back(stationA, stationB);
                cout << "Deactivated Segment " << stationA << " - " << stationB << endl;
                dont_add_segment:;
            } break;
            case '2': { // Toggle Affected Stations
                string station;
                unordered_set<string> stationNames;
                for (const auto& [name, _] : railMan.stations)
                    stationNames.insert(name);
                stationNames.insert("x"); // Cancel Option
                cin.ignore(); // Ignore \n char from previous choice.
                station = getLine("Station Name (x to Cancel):", "Invalid Station Name. Try Again.", stationNames);
                if (station == "x") break;
                for (auto it = stationsToDeactivate.begin(); it != stationsToDeactivate.end(); it++)
                    if (*it == station){
                        stationsToDeactivate.erase(it);
                        cout << "Reactivated Station " << station << endl;
                        goto dont_add_station;
                    }
                stationsToDeactivate.push_back(station);
                cout << "Deactivated Station " << station << endl;
                dont_add_station:;
            } break;
            case '3': // Clear Affected Segments
                segmentsToDeactivate.clear();
                cout << "Cleared All Segments." << endl;
                break;
            case '4': // Clear Affected Stations
                stationsToDeactivate.clear();
                cout << "Cleared All Stations." << endl;
                break;
            case 'c': // Clear All
                segmentsToDeactivate.clear();
                stationsToDeactivate.clear();
                cout << "Cleared All." << endl;
                break;
            case 's': { // See Current Settings
                cout << " - Affected Stations -\n";
                for (const auto& station : stationsToDeactivate)
                    cout << station << '\n';
                if (stationsToDeactivate.empty()) cout << "None.\n";
                cout << " - Affected Segments -\n";
                for (const auto& [stationA, stationB] : segmentsToDeactivate)
                    cout << stationA  << " - " << stationB << '\n';
                if (segmentsToDeactivate.empty()) cout << "None.\n";
                cout << flush;
            } break;
            case 'x': return false;
        }
        return true;
    });
}


// =================== //
// DATA SELECTION MENU //
// =================== //

void App::dataSelectionMenu() {
    const string title = "Data Selection";
    clear_screen();
    bool running = true;
    while (running) {
        const string projectPath = filesystem::current_path().parent_path().string() + '\\';
        string text = string("Current Path: ") + projectPath;
        cout << "\n" << getTitle(title) << string(spaceBetween, ' ') << '\n'
             << vertical << ' ' << text << '\n' << getBottomLine() << endl;

        string pathChosen;
        while (true){
            pathChosen = "";
            cout << vertical << " Path:" << flush;
            cin >> pathChosen;
            char c = pathChosen[pathChosen.size() - 1];
            if (c != '/' && c != '\\') pathChosen += '/';
            pathChosen = projectPath + pathChosen;
            cout << getBottomLine() << endl;
            if (pathChosen == "x") { running = false; break; }
            if (filesystem::exists(pathChosen)) break;
            cout << vertical << " Path Doesn't Exist. Try Again. (x to Leave)" << endl;
        }
        if (!running) break;
        bool allGood = true;
        if (!filesystem::exists(pathChosen + "stations.csv")){
            allGood = false;
            cout << vertical << ' ' << pathChosen + "stations.csv not found!" << endl;
        }
        if (!filesystem::exists(pathChosen + "network.csv")){
            allGood = false;
            cout << vertical << ' ' << pathChosen + "network.csv not found!" << endl;
        }
        if (allGood) {
            datasetPath = pathChosen;
            running = false;
        } else cout << vertical << " Path doesn't contain the necessary files." << endl;
    }
    clear_screen();
    initializeData();
}
