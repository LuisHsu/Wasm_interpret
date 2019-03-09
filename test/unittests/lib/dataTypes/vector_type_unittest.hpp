#include <skypat/skypat.h>

#define _Bool bool
extern "C" {
#include <dataTypes/vector.h>
#include <stdlib.h>
}
#undef _Bool

SKYPAT_F(vector, create_delete)
{
    // Prepare
    vector* newVector = new_vector(sizeof(int), NULL);

    // Check
    EXPECT_EQ(newVector->data, NULL);
    EXPECT_EQ(newVector->length, 0);
    EXPECT_EQ(newVector->unitSize, sizeof(int));
    EXPECT_EQ(newVector->capacity, 0);

    free_vector(newVector);
}

SKYPAT_F(vector, push_back)
{
    // Prepare
    vector* newVector = new_vector(sizeof(int), NULL);

    // Check
    int data1 = 5;
    int data2 = 8;
    int data3 = 13;
    newVector->push_back(newVector, &data1);
    EXPECT_EQ(newVector->length, 1);
    EXPECT_EQ(newVector->capacity, 1);
    newVector->push_back(newVector, &data2);
    EXPECT_EQ(newVector->length, 2);
    EXPECT_EQ(newVector->capacity, 2);
    newVector->push_back(newVector, &data3);
    EXPECT_EQ(newVector->length, 3);
    EXPECT_EQ(newVector->capacity, 4);

    free_vector(newVector);
}

SKYPAT_F(vector, pop_back)
{
    // Prepare
    vector* newVector = new_vector(sizeof(int), NULL);

    // Check
    int data1 = 5;
    int data2 = 8;
    int data3 = 13;
    newVector->push_back(newVector, &data1);
    newVector->push_back(newVector, &data2);
    newVector->push_back(newVector, &data3);
    int *check1 = (int*)newVector->pop_back(newVector);
    EXPECT_EQ(*check1, 13);
    EXPECT_EQ(newVector->length, 2);
    EXPECT_EQ(newVector->capacity, 4);
    int *check2 = (int*)newVector->pop_back(newVector);
    EXPECT_EQ(*check2, 8);
    EXPECT_EQ(newVector->length, 1);
    EXPECT_EQ(newVector->capacity, 4);
    int *check3 = (int*)newVector->pop_back(newVector);
    EXPECT_EQ(*check3, 5);
    EXPECT_EQ(newVector->length, 0);
    EXPECT_EQ(newVector->capacity, 4);
    int *check4 = (int*)newVector->pop_back(newVector);
    EXPECT_EQ(check4, NULL);
    EXPECT_EQ(newVector->length, 0);
    EXPECT_EQ(newVector->capacity, 4);

    free(check1);
    free(check2);
    free(check3);

    free_vector(newVector);
}

SKYPAT_F(vector, shrink)
{
    // Prepare
    vector* newVector = new_vector(sizeof(int), NULL);

    // Check
    int data1 = 5;
    int data2 = 8;
    int data3 = 13;
    newVector->push_back(newVector, &data1);
    newVector->push_back(newVector, &data2);
    newVector->push_back(newVector, &data3);
    EXPECT_EQ(newVector->capacity, 4);
    newVector->shrink(newVector);
    EXPECT_EQ(newVector->capacity, 3);
    newVector->push_back(newVector, &data3);
    EXPECT_EQ(newVector->capacity, 6);
    newVector->shrink(newVector);
    EXPECT_EQ(newVector->capacity, 4);

    free_vector(newVector);
}

SKYPAT_F(vector, at)
{
    // Prepare
    vector* newVector = new_vector(sizeof(int), NULL);

    // Check
    int data1 = 5;
    int data2 = 8;
    int data3 = 13;
    newVector->push_back(newVector, &data1);
    newVector->push_back(newVector, &data2);
    newVector->push_back(newVector, &data3);
    int *check1 = (int*) newVector->at(newVector, 0);
    EXPECT_EQ(*check1, 5);
    int *check2 = (int*) newVector->at(newVector, 1);
    EXPECT_EQ(*check2, 8);
    int *check3 = (int*) newVector->at(newVector, 2);
    EXPECT_EQ(*check3, 13);

    free_vector(newVector);
}

SKYPAT_F(vector, resize)
{
    // Prepare
    vector* newVector = new_vector(sizeof(int), NULL);

    // Check
    newVector->resize(newVector, 50);
    EXPECT_EQ(newVector->length, 50);
    EXPECT_EQ(newVector->capacity, 50);

    free_vector(newVector);
}