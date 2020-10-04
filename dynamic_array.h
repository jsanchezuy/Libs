#pragma once

//
// NOTE(Jorge): This only compiles in C

#include <stdlib.h>
#include <assert.h>

#define ArraySize(ARR) (*((int *) ARR - 2))
#define ArrayCapacity(ARR) (*((int *) ARR - 1))

// Allocates space for the array metadata and sets the Size and Capacity to 0.
#define ArrayInit(ARRAY)                        \
    do {                                        \
        int *Raw = malloc(2 * sizeof(int));     \
        assert(Raw);                            \
        Raw[0] = 0;                             \
        Raw[1] = 0;                             \
        ARRAY = (void*)&Raw[2];                 \
    } while(0)                                  \

// 1- Check if ARRAY is valid
// 2- Set Raw Pointer
// 3- Free
// 4- Set ARRAY to NULL
#define ArrayFree(ARRAY)                        \
    do {                                        \
        assert(ARRAY);                          \
        int *Raw = ((int*)(ARRAY) - 2);         \
        free(Raw);                              \
        ARRAY = NULL;                           \
    } while(0)                                  \

// 1- Set Raw ptr, Raw[0] == ArraySize | Raw[1] == ArrayCapacity
// 2- ArraySize + 1
#define ArrayPush(ARRAY, VALUE)                                         \
    do {                                                                \
        int *Raw = ((int*)(ARRAY) - 2);                                 \
        Raw[0] = Raw[0] + 1;                                            \
        if(Raw[1] == 0)                                                 \
        {                                                               \
            Raw[1] = 1;                                                 \
            Raw = realloc(Raw, 2 * sizeof(int) + Raw[1] * sizeof((VALUE))); \
            assert(Raw);                                                \
            (ARRAY) = (void*)&Raw[2];                                   \
        }                                                               \
        if(Raw[0] > Raw[1])                                             \
        {                                                               \
            Raw[1] = 2 * Raw[1];                                        \
            Raw = realloc(Raw, 2 * sizeof(int) + Raw[1] * sizeof((VALUE))); \
            assert(Raw);                                                \
            (ARRAY) = (void*)&Raw[2];                                   \
        }                                                               \
        ARRAY[Raw[0] - 1] = (VALUE);                                    \
    } while(0)                                                            \


// 1- Verify ARRAY is valid
// 2- Verify OUT is valid
// 3- Set Raw pointer, Raw[0] == Size | Raw[1] == Capacity
// 4- Make sure we are not trying to pop an empty array
// 5- If Array Size > 0, set OUT to the last items in the array
// 6- Decrement Array Size by 1
// 7- If ArraySize <= ArrayCapacity/3 realloc and copy
// 8- If ArraySize <= 1/4 of ArrayCapacity, realloc to be ArraySize * 2 * sizeof(Type) + Raw
// TODO(Jorge): Popping from an empty array should not crash!
#define ArrayPop(ARRAY, OUT)                                        \
    do  {                                                           \
        assert(ARRAY);                                              \
        assert(OUT);                                                \
        int *Raw = ((int*)(ARRAY) - 2);                             \
        if(Raw[0] == 0)                                             \
        {                                                           \
            assert(0 && "Array Size is 0, cannot pop the array");   \
        }                                                           \
        if(Raw[0] > 0)                                              \
        {                                                           \
            (*(OUT)) = ARRAY[Raw[0] - 1];                           \
            Raw[0] -= 1;                                            \
                                                                    \
            if(Raw[0] <= Raw[1]/4)                                  \
            {                                                       \
                Raw = realloc(Raw, 2 * sizeof(int) + Raw[0] * 2 * sizeof(*(OUT))); \
                assert(Raw);                                            \
            }                                                       \
        }                                                           \
    } while(0)                                                      \


#if MANUALLY_RUN_ARRAY_TESTS

typedef struct _player
{
    int A;
    int B;
    int C;
} player;

int main(void)
{


    { // TEST: ArrayInit should have 0 size and 0 capacity
        player *Players;
        ArrayInit(Players);
        assert(ArraySize(Players) == 0);
        assert(ArrayCapacity(Players) == 0);
    }

    { // TEST: Push one item
        player *Players;
        player PlayerOne = {};
        player PlayerTwo = {};
        player PlayerThree = {};
        ArrayInit(Players);
        ArrayPush(Players, PlayerOne);
        ArrayPush(Players, PlayerTwo);
        assert(ArraySize(Players) == 2);
        assert(ArrayCapacity(Players) == 2);
        ArrayPush(Players, PlayerThree);
        assert(ArraySize(Players) == 3);
        assert(ArrayCapacity(Players) == 4);
    }

    { // TEST: Pop one item
        player *Players;
        player PlayerOne = {};
        player PlayerTwo = {1, 2, 3};
        player Popped;
        ArrayInit(Players);
        ArrayPush(Players, PlayerOne);
        ArrayPush(Players, PlayerTwo);
        ArrayPop(Players, &Popped);
        assert(Popped.A == 1 && Popped.B == 2 && Popped.C == 3);
    }

    { // TEST: Pop from an empty array
	// TODO(Jorge): Popping from an empty array should not crash!
        // NOTE(Jorge): This fires an
        // assertion. I need to figure
        // out how to write a soft assertion routine so it can check
        // errors in a better way. In the meantime, uncomment this
        // line for the test.
        // player *Players;
        // ArrayInit(Players);
        // ArrayPop(Players, &Out);
        // player Out;
    }

    { // TEST: Check that the array is rezising upwards correctly
        player *Players;
        ArrayInit(Players);
        player PlayerOne = {};
        player PlayerTwo = {};
        player PlayerThree = {};
        player PlayerFour = {};
        player PlayerFive = {};
        ArrayPush(Players, PlayerOne);
        ArrayPush(Players, PlayerTwo);
        ArrayPush(Players, PlayerThree);
        ArrayPush(Players, PlayerFour);
        ArrayPush(Players, PlayerFive);
        assert(ArrayCapacity(Players) == 8);
    }

    { // TEST: Check that the array is resizing downwards correctly
        player *Players;
        player PlayerOne = {};
        player PlayerTwo = {};
        player PlayerThree = {};
        player PlayerFour = {};
        player PlayerFive = {};
        player PlayerSix = {};
        player Out = {};
        ArrayInit(Players);
        ArrayPush(Players, PlayerOne);
        ArrayPush(Players, PlayerTwo);
        ArrayPush(Players, PlayerThree);
        ArrayPush(Players, PlayerFour);
        ArrayPush(Players, PlayerFive);
        ArrayPush(Players, PlayerSix);
        ArrayPop(Players, &Out);
        ArrayPop(Players, &Out);
        ArrayPop(Players, &Out);
        ArrayPop(Players, &Out);
        assert(ArrayCapacity(Players) == 4);
        ArrayPop(Players, &Out);
        assert(ArrayCapacity(Players) == 2);
    }

    { // TEST: ArrayFree
        player *Players;
        ArrayInit(Players);
        player PlayerOne = {};
        player PlayerTwo = {};
        player PlayerThree = {};
        player PlayerFour = {};
        player PlayerFive = {};
        ArrayPush(Players, PlayerOne);
        ArrayPush(Players, PlayerTwo);
        ArrayPush(Players, PlayerThree);
        ArrayPush(Players, PlayerFour);
        ArrayPush(Players, PlayerFive);
        ArrayFree(Players);
        assert(Players == NULL);
    }
}

#endif
