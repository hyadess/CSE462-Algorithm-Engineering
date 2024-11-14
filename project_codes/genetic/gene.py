import random
import math
import numpy as np

class Partition:
    def __init__(self, num_nodes, num_partitions):
        self.assignment = [random.randint(0, num_partitions - 1) for _ in range(num_nodes)]
        self.fitness = 0

def calculate_fitness(graph, partition, num_partitions):
    cut_edges = 0
    partition_sizes = [0] * num_partitions

    for u in range(len(graph)):
        for v in graph[u]:
            if partition.assignment[u] != partition.assignment[v]:
                cut_edges += 1
        partition_sizes[partition.assignment[u]] += 1
    

    avg_size = len(graph) / num_partitions
    balance_penalty = sum((size - avg_size) ** 2 for size in partition_sizes)
 
    fitness = cut_edges + 10 * balance_penalty
    return fitness, cut_edges


def random_partition(num_nodes, num_partitions):
    partition = Partition(num_nodes, num_partitions)
    print(f"Initial partition assignments: {partition.assignment}")
    return partition


def crossover(parent1, parent2):
    num_nodes = len(parent1.assignment)
    child = Partition(num_nodes, len(set(parent1.assignment)))
    
    crossover_point = random.randint(0, num_nodes - 1)
    
    child.assignment[:crossover_point] = parent1.assignment[:crossover_point]
    child.assignment[crossover_point:] = parent2.assignment[crossover_point:]
    
    return child


def mutate(partition, num_partitions):
    node = random.randint(0, len(partition.assignment) - 1)
    old_partition = partition.assignment[node]
    new_partition = random.randint(0, num_partitions - 1)
    partition.assignment[node] = new_partition
    print(f"Mutated node {node} from partition {old_partition} to partition {new_partition}")

def selection(population):
    idx1, idx2 = random.sample(range(len(population)), 2)
    return population[idx1] if population[idx1].fitness < population[idx2].fitness else population[idx2]

def load_graph(filename):
    with open(filename, 'r') as f:
        num_nodes, num_edges = map(int, f.readline().split())
        graph = [[] for _ in range(num_nodes)]
        for _ in range(num_edges):
            u, v = map(int, f.readline().split())
            graph[u].append(v)
            graph[v].append(u)
    return graph

def genetic_algorithm(graph, num_partitions, population_size, generations):
    population = []


    for _ in range(population_size):
        p = random_partition(len(graph), num_partitions)
        p.fitness = calculate_fitness(graph, p, num_partitions)
        population.append(p)

    for gen in range(generations):
        new_population = []
        
        for _ in range(population_size):
            parent1 = selection(population)
            parent2 = selection(population)

            
            child = crossover(parent1, parent2)
            if random.random() < 0.2:
                mutate(child, num_partitions)

            
            child.fitness, cut_edges = calculate_fitness(graph, child, num_partitions)
            new_population.append(child)

      
        population = new_population

        
        best_partition = min(population, key=lambda p: p.fitness)
        best_fitness, best_cut_edges = calculate_fitness(graph, best_partition, num_partitions)
        print(f"Generation {gen + 1}: Best Fitness = {best_fitness}, Cut Edges = {best_cut_edges}")
        print(f"Best partition assignments: {best_partition.assignment}")

    
    return min(population, key=lambda p: p.fitness)


def main():
    filename = "graph2.txt"  
    num_partitions = 3      
    population_size = 6  
    generations = 40      

    graph = load_graph(filename)

    
    best_partition = genetic_algorithm(graph, num_partitions, population_size, generations)

    
    print("Best partition fitness:", best_partition.fitness)
    for i, partition in enumerate(best_partition.assignment):
        print(f"Node {i} -> Partition {partition}")

if __name__ == "__main__":
    main()
