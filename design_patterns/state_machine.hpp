#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <stdexcept>

template <typename TState>
class StateMachine
{
public:
    class StateNotFound : public std::invalid_argument
    {
    public:
        virtual ~StateNotFound() = default;
        StateNotFound() : std::invalid_argument("State not found") {}
    };

    class TransitionNotFound : public std::invalid_argument
    {
    public:
        virtual ~TransitionNotFound() = default;
        TransitionNotFound() : std::invalid_argument("Transition not found") {}
    };

    class ActionNotFound : public std::invalid_argument
    {
    public:
        virtual ~ActionNotFound() = default;
        ActionNotFound() : std::invalid_argument("Action not found") {}
    };

    class AlreadyRegistered : public std::invalid_argument
    {
    public:
        virtual ~AlreadyRegistered() = default;
        AlreadyRegistered() : std::invalid_argument("State already registered") {}
    };

    StateMachine() = default;
    explicit StateMachine(const TState &initial_state) : current_state_(initial_state) {}

    void addState(const TState &state)
    {
        if (std::find(states_.begin(), states_.end(), state) != states_.end())
        {
            throw AlreadyRegistered();
        }
        if (states_.empty())
        {
            current_state_ = state;
        }
        states_.push_back(state);
    }

    void addTransition(const TState &startState, const TState &finalState, const std::function<void()> &lambda)
    {
        if (std::find(states_.begin(), states_.end(), startState) == states_.end() ||
            std::find(states_.begin(), states_.end(), finalState) == states_.end())
        {
            throw StateNotFound();
        }

        transitions_[startState][finalState].push_back(lambda);
    }

    void addAction(const TState &state, const std::function<void()> &lambda)
    {
        if (std::find(states_.begin(), states_.end(), state) == states_.end())
        {
            throw StateNotFound();
        }

        actions_[state].push_back(lambda);
    }

    void transitionTo(const TState &state)
    {
        if (std::find(states_.begin(), states_.end(), state) == states_.end())
        {
            throw StateNotFound();
        }
        if (transitions_.find(current_state_) == transitions_.end() ||
            transitions_[current_state_].find(state) == transitions_[current_state_].end())
        {
            throw TransitionNotFound();
        }
        for (const auto &transition : transitions_[current_state_][state])
        {
            transition();
        }
        current_state_ = state;
    }

    void update()
    {
        if (actions_.find(current_state_) == actions_.end())
        {
            throw ActionNotFound();
        }
        else
        {
            for (const auto &action : actions_[current_state_])
            {
                action();
            }
        }
    }

private:
    TState current_state_;
    std::vector<TState> states_;
    std::map<TState, std::map<TState, std::vector<std::function<void()>>>> transitions_;
    std::map<TState, std::vector<std::function<void()>>> actions_;
};

#endif /* STATE_MACHINE_HPP */
