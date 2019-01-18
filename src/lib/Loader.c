#include <Loader.h>

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stddef.h>
#include <Stage.h>

#include <stdio.h>

static void addRequest(struct Loader_* loader, Request* request)
{
    loader->requests->push(loader->requests, request);
}

static void* run_Loader(Loader* loader)
{
    int* result = (int*) malloc(sizeof(int));
    *result = 0;

    // Run requests
    while(loader->requests->size > 0 && !loader->parent.isTerminated) {
        Request* request = NULL;
        loader->requests->pop(loader->requests, (void**)&request);
        // Check whether loaded or not
        char* loadName = (char*)((Stage*)request->stages->head->data)->input;
        _Bool loaded = 0;
        for(listNode* cur = loader->loadedList->head; cur != NULL; cur = cur->next) {
            if(!strcmp(loadName, (char*)cur->data)) {
                loaded = 1;
                break;
            }
        }
        if(loaded) {
            free(loadName);
            request->free(request);
            continue;
        }
        loader->loadedList->push_back(loader->loadedList, loadName);
        // Run stages
        while(request->stages->size > 1) {
            Stage* stage = NULL;
            request->stages->pop(request->stages, (void**)&stage);
            *result = stage->run(stage);
            free(stage);
            if(*result) {
                request->free(request);
                return result;
            }
        }
        loader->decodedStack->push(loader->decodedStack, request);
    }
    // Run instanciator
    while(loader->decodedStack->size > 0 && !loader->parent.isTerminated) {
        Request* request = NULL;
        loader->decodedStack->pop(loader->decodedStack, (void**)&request);
        Stage* stage = NULL;
        request->stages->pop(request->stages, (void**)&stage);
        *result = stage->run(stage);
        request->free(request);
        free(stage);
        if(*result) {
            return result;
        }
    }
    return result;
}

static int activate_Loader(Component* component)
{
    return pthread_create(&component->thread, NULL, (void* (*) (void*))run_Loader, (void*)component);
}

static void terminate_Loader(Component* component)
{
    component->isTerminated = 1;
}

static void freeRequest(Request* request)
{
    request->free(request);
}

Loader* new_Loader()
{
    Loader* newLoader = (Loader*)malloc(sizeof(Loader));
    newLoader->parent.activate = activate_Loader;
    newLoader->parent.terminate = terminate_Loader;
    newLoader->parent.isTerminated = 0;
    newLoader->addRequest = addRequest;
    newLoader->loadedList = new_list(free);
    newLoader->decodedStack = new_stack((void (*)(void*))freeRequest);
    newLoader->requests = new_queue((void (*)(void*))freeRequest);
    return newLoader;
}

void free_Loader(Loader* loader)
{
    free_list(loader->loadedList);
    free_stack(loader->decodedStack);
    free_queue(loader->requests);
    free(loader);
}
