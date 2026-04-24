/*
 * Metodología de la Programación
 * Curso 2025/2026
 */

/**
 * @file ArrayClustering.cpp
 * @author Silvia Acid Carrillo <acid@decsai.ugr.es>
 * @author Andrés Cano Utrera <acu@decsai.ugr.es>
 * @author Luis Castillo Vidal <L.Castillo@decsai.ugr.es>
 *
 * Created on 22 de octubre de 2025, 11:57
 */

#include "Clustering.h"
#include "ArrayClustering.h"

/**
 * @brief Initializes the provided ArrayClustering with a given capacity. It
 * allocates a dynamic array of Clustering objects with the provided capacity,
 * sets the capacity field and initializes the size field to 0.
 * @throw std::out_of_range Throws a std::out_of_range exception if the
 * provided capacity is less than or equal to 0.
 * @param arrayClustering The ArrayClustering to initialize. Output parameter
 * @param capacity The initial capacity of the array. Input parameter
 */
void InitializeArrayClustering(ArrayClustering &arrayClustering,
                               int capacity){
                                if (capacity <= 0) {
                                    throw std::out_of_range("Capacity must be greater than 0");
                                }
                                arrayClustering.clustering = new Clustering[capacity];
                                arrayClustering.capacity = capacity;
                                arrayClustering.size = 0;
                               }

/**
 * @brief Deallocates the dynamic array of Clustering objects in the provided
 * ArrayClustering. It also sets the clustering pointer to nullptr, and the
 * capacity and size fields to 0.
 * @param arrayClustering The ArrayClustering to deallocate. Output parameter
 */
void DeallocateArrayClustering(ArrayClustering &arrayClustering){
    delete[] arrayClustering.clustering;
    arrayClustering.clustering = nullptr;
    arrayClustering.capacity = 0;
    arrayClustering.size = 0;
}

/**
 * @brief Searches for a given Clustering object in the provided
 * ArrayClustering. The search is performed by comparing each Clustering object
 * in the array with the provided Clustering object using the
 * Clustering::isEquivalentTo() method.
 * @param arrayClustering The ArrayClustering where to search will be
 * performed. Input parameter
 * @param clustering The Clustering object to search for. Input parameter
 * @return The position of the Clustering object in the array if found;
 * -1 otherwise.
 */
int FindArrayClustering(ArrayClustering &arrayClustering,
                                        const  Clustering &clustering){
    for (int i = 0; i < arrayClustering.size; i++) {
        if (arrayClustering.clustering[i].isEquivalentTo(clustering)) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Appends the given Clustering object to the provided ArrayClustering
 * object. The Clustering object is only appended if the array does not
 * contain another Clustering object equivalent to the one being inserted.
 * If the array is full and the Clustering object must be appended to the array,
 * this function resizes the array with a capacity equal to the current
 * capacity plus an extra block of size equal to
 * ARRAY_CLUSTERING_CAPACITY_INCREMENT.
 * @param arrayClustering The ArrayClustering where the Clustering object will
 * be appended. Output parameter
 * @param clustering The Clustering object to append. Input parameter
 */
void AppendArrayClustering(ArrayClustering &arrayClustering,
                                            const Clustering &clustering){
                                                if (FindArrayClustering(arrayClustering, clustering) != -1) {
                                                    return;
                                                }
                                                if (arrayClustering.size >= arrayClustering.capacity) {
                                                    int newCapacity = arrayClustering.capacity + ARRAY_CLUSTERING_CAPACITY_INCREMENT;
                                                    Clustering* newArray = new Clustering[newCapacity];
                                                    for (int i = 0; i < arrayClustering.size; i++) {
                                                        newArray[i] = arrayClustering.clustering[i];
                                                    }
                                                    delete[] arrayClustering.clustering;
                                                    arrayClustering.clustering = newArray;
                                                    arrayClustering.capacity = newCapacity;
                                                }
                                                arrayClustering.clustering[arrayClustering.size] = clustering;
                                                arrayClustering.size++;
                                            }

/**
 * @brief Sorts the Clustering objects in the provided ArrayClustering in
 * ascending order based on their sum of within-cluster variances. If two
 * Clustering objects have the same sum of within-cluster variances, they are
 * further sorted by their number of iterations in ascending order.
 * @param arrayClustering The ArrayClustering to sort. Output parameter
 */
void SortArrayClustering(ArrayClustering &arrayClustering){
    Clustering auxiliar;
    for (int i = 0; i < arrayClustering.size - 1; i++) {
        for (int j = i + 1; j < arrayClustering.size; j++) {
            if (arrayClustering.clustering[i].getSumWCV() > arrayClustering.clustering[j].getSumWCV()) {
                auxiliar = arrayClustering.clustering[i];
                arrayClustering.clustering[i] = arrayClustering.clustering[j];
                arrayClustering.clustering[j] = auxiliar;
            } else if (arrayClustering.clustering[i].getSumWCV() == arrayClustering.clustering[j].getSumWCV()) {
                if (arrayClustering.clustering[i].getNumIterations() > arrayClustering.clustering[j].getNumIterations()) {
                    auxiliar = arrayClustering.clustering[i];
                    arrayClustering.clustering[i] = arrayClustering.clustering[j];
                    arrayClustering.clustering[j] = auxiliar;
                }
            }
        }
    }
}
