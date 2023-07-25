#pragma once

#include "State.h"
#include "io.h"

#include <iostream>
#include <set>
#include <queue>

class Solver
{
public:
    template <class State_t>
    static Moves SolveBFS(const State_t& initialState)
    {
        std::cout << "Solving..." << std::endl << initialState;

        Validate(initialState);
        if (initialState.IsGoalState()) return {}; // no moves required. Initial state is goal state.

        using Node = std::pair<State_t, Moves>;

        //TODO: define OPEN SET correctly
        auto stateCompare = [](const State_t& first, const State_t& second) -> bool
        {
            return first.GetData() < second.GetData();
        };
        std::queue<Node> openSet;
        openSet.push({ initialState, {} });

        //TODO: define CLOSED SET correctly
        //std::set<State_t> closedSet;
        std::set<State_t, decltype(stateCompare)> closedSet(stateCompare);

        // TODO: Create a comparator so std::set can work with the State instances.
        // It doesn't really make sense to compare states otherwise...
       /* auto stateCompare = [](const State_t& first, const State_t& second) -> bool
        {
            return first.GetData() < second.GetData();
        };*/

        while (!openSet.empty())
        {
            // TODO: Do it nicer , who is first? second? structure binding
            auto currentNode = openSet.front();
            auto&& [currentState, currentMoves] = currentNode;
            /*auto&& currentState = currentNode.first;
            auto&& currentMoves = currentNode.second;*/
            openSet.pop();

            // some logging
            static size_t maxDepth = 0;
            if (currentMoves.size() > maxDepth)
            {
                maxDepth = currentMoves.size();
                std::cout << "Max Depth: " << maxDepth << std::endl;
            }
            // end logging

            if (currentState.IsGoalState())
            {
                std::cout << "Visited: " << closedSet.size() << std::endl;
                return currentMoves; // leaving cycle statement
            }

            closedSet.insert(currentState);

            for (auto&& childMovePair : currentState.GetChildren())
            {
                
                auto&& [childState, move] = childMovePair;
                
                if (!closedSet.contains(childState)) // TODO: use C++20
                {
                    Moves childMoves = currentMoves;
                    childMoves.push_back(move);
                    openSet.push({ std::move(childState), std::move(childMoves) });
                }
            }
        }

        throw std::runtime_error("Couldn't solve");
    }

private:
    //musthaveTODO implement Validate correctly
    template <class State_t>
    static void Validate(const State_t& state)
    {
        if (!state.IsValid())
        {
            throw std::runtime_error("state not valid");
        }
        if (!state.IsSolvable())
        {
            throw std::runtime_error("state not solvable");
        }
        
    }
};
