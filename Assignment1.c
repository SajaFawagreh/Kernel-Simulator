/**
 * SYSC 4001 - Operating Systems
 * Lab section: L12
 * Assignment 1, Part 2 
 * Design and Implementation of a Kernel Simulator
 * 
 * @author	Saja Fawagreh
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// Defines a structure for the PCB with the necessary information
typedef struct pcb {	
    int pid;                    
    int arrival_time;
    int total_CPU_time;
    int remaining_CPU_time;
    int io_frequency;
    int io_duration;
    int waiting_time;
} pcb_t;

// Defines a structure for a node to link the PCBs
typedef struct node {	
    pcb_t *pcb;
    struct node *next;
} node_t;

// Defines a structure for the queue
typedef struct {  
    node_t *front;
    node_t *rear;
    int size;
} queue_t;

// This function allocates a queue on the heap and returns a pointer to it
queue_t *alloc_queue(void) {
    queue_t *queue = malloc(sizeof(queue_t));  
    assert(queue != NULL);
    queue->front = NULL;  
    queue->rear = NULL;  
    queue->size = 0;  
    return queue;
}

/**
 * This function allocates a PCB structure on the heap and returns a pointer to it.
 *  
 * @param pid            The process ID of the PCB.
 * @param arrival_time   The arrival time of the PCB at the ready.
 * @param total_CPU_time The total CPU time of the PCB.
 * @param io_freq        The I/O frequency of the PCB.
 * @param io_dur         The I/O duration of the PCB.
 * @return               A pointer to the PCB structure created.
 */
pcb_t *new_pcb(int pid, int arrival_time, int total_CPU_time, int io_freq, int io_dur) {
    pcb_t *pcb = malloc(sizeof(pcb_t));
    assert(pcb != NULL);
    pcb->pid = pid;
    pcb->arrival_time = arrival_time;
    pcb->total_CPU_time = total_CPU_time;
    pcb->remaining_CPU_time = total_CPU_time;
    pcb->io_frequency = io_freq;
    pcb->io_duration = io_dur;
    pcb->waiting_time = 0;
    return pcb;
}

/** 
 * This function enqueues (appends) a node to the end of the queue
 * 
 * @param queue   A pointer to the queue you want to enqueue a node to.
 * @param pcb     A pointer to the node you want to enqueue to the queue
 */ 
void enqueue(queue_t *queue, node_t *pcb)
{
	assert(queue!=NULL);
	if(queue->size == 0){
		queue->front = pcb;
	}
	else{
		queue->rear->next = pcb;
	}
    queue->rear = pcb;
	queue->size++;
}

/** 
 * This function dequeues (removes) the node from the front of the queue and returns a pointer to it.
 * 
 * @param queue   A pointer to the queue you want to dequeue from.
 * @return        A pointer to the dequeued node, or NULL if the queue is empty.
 */
node_t *dequeue(queue_t *queue)
{
    assert(queue!=NULL);
    assert(queue->size!=0);
    node_t *r = queue->front;
    queue->front = queue->front->next;
    if(queue->size == 1){
        queue->rear = NULL;  
    } 
	queue->size--;
	return r;
}

/** 
 * This function dequeues (removes) a specific node from the queue and returns a pointer to it.
 * 
 * @param queue          A pointer to the queue you want to dequeue the node from.
 * @param nodeToRemove   A pointer to the node you want to remove.
 * @return               A pointer to the removed node, or NULL if the node is not found or the queue is empty.
 */
node_t *dequeueNode(queue_t *queue, node_t *nodeToRemove) {
    assert(queue != NULL);
    assert(queue->size!=0);

    node_t *current = queue->front;
    node_t *prev = NULL;

    while (current != NULL) {
        if (current == nodeToRemove) {
            if (prev == NULL) {
                queue->front = current->next;
            } else {
                prev->next = current->next;
            }
            if (current == queue->rear) {
                queue->rear = prev;
            }
            queue->size--;
            current->next = NULL; 
            return current;
        }
        prev = current;
        current = current->next;
    }

    return NULL;
}

/** 
 * This function scans a CSV file, creates a PCB structure for each entry, enqueues them into a queue, and returns a pointer to the queue.
 * 
 * @param filename   The name of the input CSV file.
 * @return           A pointer to the queue containing PCB structures.
 */
queue_t *read_CSV_file(const char filename[]){
    pcb_t *pcb;
    node_t *p;
    queue_t *queue = alloc_queue();
    int pid, arrivalTime, totalCpuTime, ioFrequency, ioDuration;

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    // Create a character buffer to store input
    char buffer[1024];
    // Read a line of text from file and store it in buffer
    fgets(buffer, sizeof(buffer), file);

    // Read data from the file in a loop as long as fscanf is able to extract all five integers from the line
    while (fscanf(file, "%d,%d,%d,%d,%d", &pid, &arrivalTime, &totalCpuTime, &ioFrequency, &ioDuration) == 5) {
        p = malloc(sizeof(node_t));
        pcb = new_pcb(pid, arrivalTime, totalCpuTime, ioFrequency, ioDuration); // Create a new PCB structure with the extracted values
        p->pcb = pcb; // Assigns the PCB value to PCB structure created
        p->next = NULL; // Assigns the next value to NULL since it will be the last element in the queue
        enqueue(queue, p); // Enqueue the node containing the PCB into the queue
    }

    fclose(file);

    return queue;
}

/**
 * This function writes the provided information for a PCB to the output file.
 *
 * @param filename   The name of the output file.
 * @param time       The time at which the PCB switches states.
 * @param pid        The process ID (PID).
 * @param oldState   The old state of the PCB.
 * @param newState   The new state of the PCB.
 */
void write_text_file(const char *filename, int time, int pid, const char *oldState, const char *newState) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    fprintf(file, "%-5d %-4d %-10s %-10s\n", time, pid, oldState, newState);

    fclose(file);
}

/**
 * This function simulates an OS kernel, processing PCB state transitions.
 * 
 * @param new_queue    A pointer to the queue containing PCBs to process.
 * @param filename     The name of the output file for logging state transitions.
 */
void flow_process(queue_t *new_queue, const char *filename){
    queue_t *ready_queue = alloc_queue();
    queue_t *running_queue = alloc_queue();
    queue_t *waiting_queue = alloc_queue();
    queue_t *terminated_queue = alloc_queue();

    node_t *current;             // A pointer to the current node in the loop
    node_t *removed;             // A pointer to the node being removed
    int running_time = 0;        // The running time of the PCB in the running state
    int size = new_queue->size;  // The number of PCBs, which is the size of the new_queue
    int clock = 0;               // The timer
    
    // The while loop won't stop until all PCBs are terminated.
    while(terminated_queue->size < size){

        // Check for PCBs in the new queue and transitions them to the ready state if their arrival time matches the current clock time
        if(new_queue->size != 0){
            current =  new_queue->front;
            int new_size = new_queue->size;

            for(int i = 0; i < new_size; i++) {
                if (current->pcb->arrival_time == clock) {
                    node_t *next = current->next;
                    removed = dequeue(new_queue);
                    enqueue(ready_queue, removed);
                    write_text_file(filename, clock, removed->pcb->pid, "New", "Ready");
                    current = next;
                }

                else{
                    current = current->next;
                }
            }
        }

        // Check for PCBs in the waiting queue and transition them to the ready state if their waiting time matches their I/O duration
        if(waiting_queue->size != 0){
            current =  waiting_queue->front;
            int waiting_size = waiting_queue->size;

            for(int i = 0; i < waiting_size; i++) {
                
                current->pcb->waiting_time++;

                if (current->pcb->io_duration == current->pcb->waiting_time) {
                    current->pcb->waiting_time = 0;
                    node_t *next = current->next;
                    removed = dequeueNode(waiting_queue, current);
                    enqueue(ready_queue, removed);
                    write_text_file(filename, clock, removed->pcb->pid, "Waiting", "Ready");
                    current = next;
                }
               
                else{
                    current = current->next;
                }
            } 
        } 

        // Checks the PCB in the running queue and transitions them to the terminated state if their remaining CPU time is 0 
        if(running_queue->size != 0){
            running_time++;
            running_queue->front->pcb->remaining_CPU_time--;
            
            if (running_queue->front->pcb->remaining_CPU_time == 0) {
                removed = dequeue(running_queue);
                enqueue(terminated_queue, removed);
                write_text_file(filename, clock, removed->pcb->pid, "Running", "Terminated");
            }
        
        // Checks the PCB in the running queue and transitions them to the waiting queue if their I/O frequency matches the running time
            else if(running_time == running_queue->front->pcb->io_frequency){
                removed = dequeue(running_queue);
                enqueue(waiting_queue, removed);
                write_text_file(filename, clock, removed->pcb->pid, "Running", "Waiting");      
            }
        }

        // Checks the ready queue for PCBs and transitions the first PCB in the ready queue to the running state if the running state is available
        if(ready_queue->size != 0){
            if (running_queue->size == 0) {
                removed = dequeue(ready_queue);
                enqueue(running_queue, removed);
                running_time = 0;
                write_text_file(filename, clock, removed->pcb->pid, "Ready", "Running");
            }
        }

        // Increments the timer after each loop
        clock++; 
    }

    // Deallocates memory after jobs are terminated 
    free(ready_queue);
    free(waiting_queue);
    free(running_queue);
    free(terminated_queue);
    free(current);
    free(removed);
}

int main(int argc, char *argv[]){

    queue_t *new_queue = alloc_queue();
    new_queue = read_CSV_file("test_case_1.csv");
    FILE *file = fopen("output_1.txt", "w");
    fprintf(file, "%-5s %-4s %-10s %-10s\n", "Time", "PID", "Old State", "New State");
    fclose(file);
    flow_process(new_queue, "output_1.txt");

    free(new_queue);
    free(file);

    return 0;
}