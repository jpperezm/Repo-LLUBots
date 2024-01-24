/**
 * @brief Enumeration representing the different states of the LLUBot.
 * The LLUBot transitions through various states as it navigates its path,
 * searching for a 'home', communicating with other LLUBots, and returning to its starting position.
 */
enum LLUBotState {
  kIdle, 
  kSearch,
  kAtHomeFirstTry, 
  kFoundHome, 
  kReturn,
  kIdleRoundabout,
  kRoundabout,
  kLeaveRoundabout,
  kGoingHome,
  kAtHome,
  kConfig,
  kLineFollowerTest,
  kEmergencyStop
};

/**
 * @brief Current state of the LLUBot, representing its mode of operation in the journey.
 */
extern LLUBotState currentState;

/**
 * @brief Flag indicating if a start command has been received, signaling the LLUBot to begin its journey.
 */
extern bool startCommandReceived;

/**
 * @brief Flag indicating if a reset command has been received, instructing the LLUBot to reset its state and operations.
 */
extern bool resetCommandReceived;

/**
 * @brief Flag indicating if the LLUBot has been switched to configuration mode, typically for setup or diagnostics.
 */
extern bool configModeActivated;

/**
 * @brief Flag indicating if the LLUBot's current location has been transmitted, usually for coordination with other LLUBots.
 */
extern bool LLUBotLocationSent;

/**
 * @brief Represents the total number of LLUBots participating in the task, used for coordination and strategy.
 */
extern int numberOfLLUBots;

/**
 * @brief Identifier for the 'home' the LLUBot is assigned to find, unique for each LLUBot.
 */
extern int homeName;

/**
 * @brief Identifier for the initial street or position from where the LLUBot starts its journey.
 */
extern int initialStreetName;

/**
 * @brief Identifier for the goal street or position the LLUBot aims to reach after finding its home.
 */
extern int goalStreetName;

/**
 * @brief Counter for the number of LLUBots currently on the roundabout, used for traffic management and collision avoidance.
 */
extern int numberOfLLUBotsOnRoundabout;

/**
 * @brief Unique identifier for each LLUBot, used for communication and identification purposes.
 * This is typically the color in Spanish of the LLUBot.
 */
extern String LLUBotID;

/**
 * @brief Flag indicating whether the line follower test is currently active, used for calibration or testing the line following functionality.
 */
extern bool lineFollowerTest;

/**
 * @brief Flag indicating if the emergency stop feature is active, used to halt all operations of the LLUBot immediately for safety.
 */
extern bool emergencyStop;

/**
 * @brief Updates the state of the LLUBot.
 * This function manages the state transitions of the LLUBot based on various flags and conditions.
 * It incorporates the logic for the LLUBot's journey, communication, and coordination with other LLUBots.
 */
void updateRobotState();

/**
 * @brief Handles the current state of the LLUBot.
 * This function executes the specific actions associated with the current state of the LLUBot.
 * It is responsible for the operational behavior in each state.
 */
void handleRobotState();