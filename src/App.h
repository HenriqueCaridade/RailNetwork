
#ifndef RAILNETWORK_APP_H
#define RAILNETWORK_APP_H

#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include "RailManager.h"

/**
 * App represents the application (I/O).
 *
 * App takes care of the input and output of the program, it has functions to get multiple different ways to get input and has different ways of outputting information.
 */
class App {
    const static int titleSize = 65;
    const static int spaceBetween = 2;
    const static char corner = 'o';
    const static char vertical = '|';
    const static char horizontal = '-';

    std::string datasetPath;
    RailManager railMan;


    /**
     * Gets a Floating Point input from the user.
     * @param const std::string& question
     * @param const std::string& invalidMessage
     * @param bool (*isValid)(double)
     * @return std::string
     */
    static std::string getDoubleString(const std::string& question, const std::string& invalidMessage, bool (*isValid)(double));
    /**
     * Gets a General input from the user.
     * @tparam T
     * @param const std::string& question
     * @param const std::string& invalidMessage
     * @param const std::vector<T>& validOptions
     * @return T
     */
    template <class T> static T getInput(const std::string& question, const std::string& invalidMessage, const std::unordered_set<T>& validOptions);
    /**
     * Gets a Multi Word input from the user.
     * @param const std::string& question
     * @param const std::string& invalidMessage
     * @param const std::unordered_set<std::string>& validOptions
     * @return std::string
     */
    static std::string getLine(const std::string& question, const std::string& invalidMessage, const std::unordered_set<std::string>& validOptions);
    /**
     * Fills the line with the correct size.
     * @param const std::string& str
     * @return std::string
     */
    static std::string fillLine(const std::string& str);
    /**
     * Pads string with the necessary characters to complete the menu line format.
     * @param const std::string& str
     * @return std::string
     */
    static std::string getMenuLine(const std::string& str);
    /**
     * Centers the string with the title line format.
     * @param const std::string& title
     * @return std::string
     */
    static std::string getTitle(const std::string& title);
    /**
     * Returns the bottom line string.
     * @return std::string
     */
    static std::string getBottomLine();
    /**
     * Returns the correct string to build the menu.
     * @param const std::vector<std::string>& options
     * @param int index
     * @return std::string
     */
    static std::string getOption(const std::vector<std::string>& options, int index);
    /**
     * Draws the menu on the console with title and options.
     * @param const std::string& title
     * @param const std::vector<std::string>& options
     */
    static void drawMenu(const std::string& title, const std::vector<std::string>& options);
    /**
     * Main Menu. (Calls runMenu)
     */
    void mainMenu();
    /**
     * Data Selection Menu.
     */
    void dataSelectionMenu();
    /**
     * Data Selection Menu. (Calls runMenu)
     */
    void basicServiceMenu();
    /**
     * Data Selection Menu. (Calls runMenu)
     */
    void costMenu();
    /**
     * Data Selection Menu. (Calls runMenu)
     */
    void reliabilityMenu();
    /**
     * Runs a menu with the given title, image, options and for every valid option calls f(option) to process choice.
     * @tparam Lambda
     * @param const std::string& title
     * @param const std::vector<std::pair<char, std::string>>& options
     * @param Lambda f
     */
    template <typename Lambda>
    void runMenu(const std::string& title, const std::vector<std::pair<char, std::string>>& options, Lambda f, bool clearFirst = true, bool clearLast = true);

    void initializeData();
public:
    /**
     * Default constructor.
     */
    App();
    /**
     * Starts app. (Calls mainMenu()).
     */
    void start();
};


#endif //RAILNETWORK_APP_H