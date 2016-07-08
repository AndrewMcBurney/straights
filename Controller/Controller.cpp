#include "Controller.h"
#include "../Global/Context.h"
#include "../Model/Deck.h"

using namespace std;

Controller::Controller(unique_ptr<Straights> model, unique_ptr<TextView> textView)
    : model(move(model)), textView(move(textView)) {}

void Controller::initialize() {

}

void Controller::startGame() {
    vector<char> playerTypes = textView->getPlayerTypes();

    // Given the 4 player types, create the players in the game
    for (int i = 0; i < 4; i++) {
        if (playerTypes[i] == 'c')
            model->addComputerPlayer(i+1);
        else
            model->addHumanPlayer(i+1);
    }

    model->deal();

    // Print a summary of the game before starting
    textView->printObject<Straights>(*model);

    while (true) {
        // Start the next round
        TurnResult turnResult = model->next();

        if (turnResult.getType() == TurnResult::REQUIRE_HUMAN_INPUT) {
            // We require more input from the user to complete the round

            // Print the context for the user to decide
            textView->printObject<TurnContext>(model->getTurnContext());

            bool validInputProvided = false;
            while (!validInputProvided) {
                // Read commands from the player

                Command input = textView->getCommand();
                switch (input.type) {
                    case Command::DECK: {
                        textView->printObject<Deck>(model->getDeck());
                        break;
                    }
                    case Command::QUIT: {
                        return;
                    }
                    case Command::RAGEQUIT: {
                        model->automateCurrentPlayer();
                        textView->printRagequit(model->returnCurrentPlayer()->getID());
                        input = Command();
                    }
                    default: {
                        try {
                            turnResult = model->next(input);
                            validInputProvided = true;
                        } catch (const invalid_argument &e) {
                            textView->printObject<string>(e.what());
                            textView->printNewLine();
                        }
                        break;
                    }
                }
            }
        }

        // Print a summary of the move made
        textView->printObject<TurnResult>(turnResult);

        if (turnResult.getStatus() == TurnResult::ROUND_COMPLETE ||
            turnResult.getStatus() == TurnResult::GAME_COMPLETE) {

            textView->printObject<RoundContext>(model->getRoundContext());

            // Prepare the next round
            model->clearRound();
            model->deal();

            // Print a summarry of the game
            textView->printObject<Straights>(*model);
        }

        if (turnResult.getStatus() == TurnResult::GAME_COMPLETE)
            // The game is over
            return;
    }
}
