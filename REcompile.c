#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int indexRegex;
char *regex;
int length;

int state;
const char BR = 'B';
const char WC = 'W';

// the table.
char states[256];
int next1[256];
int next2[256];

int expression();
int term();
int factor();
void printTable();
void setState(int s, char c, int n1, int n2);

int REcompile(char *inputRegex)
{
    regex = inputRegex;
    length = strlen(regex);

    // indexRegex in regex array we are looking at
    indexRegex = 0;

    // which column in the table we are building
    state = 1;

    int v = expression();

    setState(0, BR, v, v);

    // setState(state, BR, -1, -1);

    printTable();

    return 0;
}

void printTable()
{
    printf("PRINTING TABLE\n");

    for (int i = 0; i < state; i++)
    {
        printf(" %c", states[i]);
    }
    printf("\n");

    for (int i = 0; i < state; i++)
    {
        printf(" %d", next1[i]);
    }
    printf("\n");

    for (int i = 0; i < state; i++)
    {
        printf(" %d", next2[i]);
    }
    printf("\n");
}

void setState(int s, char c, int n1, int n2)
{
    /**
     * Update the table with new state values.
     *
     * @param s  the state number to update, ie. which row in the table
     * @param c  the character that needs to be read for this state to jump to the
     *           next state
     * @param n1 the next state to jump to if c is seen
     * @param n2 will be different to n1 only on a branch state.
     */

    states[s] = c;
    next1[s] = n1;
    next2[s] = n2;
}

void setStartingState(int oldState, int newState, int rangeStart, int rangeEnd)
{
    /**
     * A helpful function that lets me update where states in the table point to,
     * It is used after states are created, for example an alturnation term may look
     * like the table below.
     * This function is used for pointing state 5 to 8. Also what used to point to
     * state 5, to state 6.
     * The range variables give us the range within the table to scan for changes.
     *
     * 5, 6, 7
     * a, BR, b
     * 8, 5, 8
     * 8, 7, 8
     *
     * @param oldState   What the state is currently pointing to
     * @param newState   The state to change it to
     * @param rangeStart The start state to start checking
     * @param rengeEnd   End of range to check
     */

    if (rangeEnd > state)
        rangeEnd = state;

    for (int i = rangeStart; i < rangeEnd; i++)
    {
        if (next1[i] == oldState)
        {
            next1[i] = newState;
        }
        if (next2[i] == oldState)
        {
            next2[i] = newState;
        }
    }
}

void invalidSyntax()
{
    /**
     * Endures the program exits gracefully when a syntax error in the regex is
     * caught by the program.
     */
    printf("INVALID SYNTAX\n");
}

int isvocab(char c)
{
    /**
     * Check if a given character is a volcabolary item.
     * Used for parsing and error control.
     *
     * @param c the character to check
     * @return true or false whether c is valid volcabolary
     */

    return (c == '(' || c == ')' || c == '+' || c == '?' || c == '*' || c == '\\' || c == '|' || c == BR || c == WC) ? 0 : 1;
}

int expression()
{
    // options:
    // E -> T
    // E -> TE

    int r = term();

    if (indexRegex == length)
        return r;

    if (isvocab(regex[indexRegex]) == 1 || regex[indexRegex] == '(' || regex[indexRegex] == '\\')
    {
        int startState = state;
        int entryPoint = expression();

        setStartingState(startState, entryPoint, 0, startState);
        // match any twos with threes
    }
    else if (regex[indexRegex] == ')')
    {
        return r;
    }
    else
    {
        invalidSyntax();
    }
    return r;
}

int term()
{
    // options:
    // T -> F
    // T -> F*
    // T -> F|T
    // T -> F?
    // T -> F+

    int factor_ = factor();
    int origin;

    if (indexRegex == length)
        return factor_;

    switch (regex[indexRegex])
    {
    case '*':
        // build branch
        setState(state, BR, factor_, state + 1);
        indexRegex++;
        state++;

        // return new start state
        return state - 1;

    case '|':

        origin = state;

        setState(state, BR, factor_, state + 1);

        indexRegex++;
        state++;

        int prevTerm = term();

        // make the factor_ point to after the alternation, rather than pointing at the
        // branching point.
        setStartingState(origin, state, 0, origin);

        // make the branch point to the start of the next term, rather than the next
        // state
        setStartingState(origin + 1, prevTerm, 0, prevTerm - 1);

        return origin;

    case '?':
        // zero or one time
        setState(state, BR, factor_, state + 1);
        indexRegex++;
        state++;

        // make the factor_ point to next state aswell
        setStartingState(state - 1, state, 0, state - 1);

        return state - 1;

    case '+':
        // one or more times
        origin = state;
        // setState(state, regex[indexRegex - 1], state + 1, state + 1);
        setState(state, BR, factor_, state + 1);
        indexRegex++;
        state++;

        // return new start state
        return origin;

    default:
        return factor_;
    }
}

int factor()
{
    // options:
    // F -> v
    // F -> (E)
    // F -> \x

    setState(state, regex[indexRegex], state + 1, state + 1);
    indexRegex++;
    state++;
    return state - 1;

    if (indexRegex == length)
        invalidSyntax();

    if (regex[indexRegex] == '.')
    {
        setState(state, WC, state + 1, state + 1);
        indexRegex++;
        state++;
        return state - 1;
    }

    else if (isvocab(regex[indexRegex]) == 1)
    {
        setState(state, regex[indexRegex], state + 1, state + 1);
        indexRegex++;
        state++;
        return state - 1;
    }

    else if (regex[indexRegex] == '\\')
    {
        indexRegex++;
        setState(state, regex[indexRegex], state + 1, state + 1);
        indexRegex++;
        state++;
        return state - 1;
    }

    else if (regex[indexRegex] == '(')
    {
        indexRegex++;
        int r = expression();
        if (indexRegex == length)
            invalidSyntax();

        if (regex[indexRegex] == ')')
        {
            indexRegex++;
            return r;
        }
        else
        {
            invalidSyntax();
        }
    }
    else
    {
        invalidSyntax();
    }

    invalidSyntax();
    return -1;
}
