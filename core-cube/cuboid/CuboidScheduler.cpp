#include "kylin/cuboidScheduler.hpp"

CuboidScheduler::CuboidScheduler(CubeDesc* cubeDesc) {
    this->cubeDesc = cubeDesc;
    int size = cubeDesc->getRowKey()->getRowKeyColumns()->length;
    this.max = (long) pow(2, size) - 1;

    std::pair<std::set<long>, std::map<long, std::list<long>>> pair = buildTreeBottompUp();
    this->allCuboidIds = pair.getFirst();
    this->parent2child = pair.getSecond();
}

int CuboidScheduler::getCuboidCount() { return allCuboidIds.size(); }

std::list<long> CuboidScheduler::getSpanningCuboid(long cuboid) {
    if (cuboid > max || cuboid < 0) {
        throw "Cuboid" + std::to_string(cuboid) + " is out of scope 0 - " + std::to_string(max);
    }

    std::list<long> spanning = parent2child[cuboid];
    if (spanning == NULL) {
        std::list<long> empty;
        return empty;
    }
    return spanning;
}

std::set<long> CuboidScheduler::getAllCuboidIds() { return allCuboidIds; }

bool CuboidScheduler::isValid(long requestCuboid) {
    if (allCuboidIds.find(requestCuboid) != allCuboidIds.end())
        return true;
    else
        return false;
}

/** Returns a valid cuboid that best matches the request cuboid. */
long CuboidScheduler::findBestMatchCuboid(long cuboid) { return findBestMatchCuboid1(cuboid); }

long CuboidScheduler::findBestMatchCuboid1(long cuboid) {
    if (isValid(cuboid)) {
        return cuboid;
    }

    std::list<long> onTreeCandidates;
    std::list<AggregationGroup*> aggregationGroups = cubeDesc->getAggregationGroups();
    for (std::list<AggregationGroup*>::iterator it = aggregationGroups.begin(); it != aggregationGroups.end(); ++it) {
        long candidate = (*it)->translateToOnTreeCuboid(cuboid);
        if (candidate != NULL) {
            onTreeCandidates.push_back(candidate);
        }
    }

    if (onTreeCandidates.size() == 0) {
        return getBaseCuboidId();
    }

    // long onTreeCandi = min(onTreeCandidates, Cuboid.cuboidSelectComparator);
    long onTreeCandi = onTreeCandidates;
    if (isValid(onTreeCandi)) {
        return onTreeCandi;
    }

    return doFindBestMatchCuboid1(onTreeCandi);
}

long CuboidScheduler::doFindBestMatchCuboid1(long cuboid) {
    long parent = getOnTreeParent(cuboid);
    while (parent > 0) {
        std::set<long> allCuboids = cubeDesc->getAllCuboids();
        if (allCuboids.find(parent) != allCuboids.end()) {
            break;
        }
        parent = getOnTreeParent(parent);
    }

    if (parent <= 0) {
        throw "Can't find valid parent for Cuboid " + std::to_string(cuboid);
    }
    return parent;
}

long CuboidScheduler::getOnTreeParent(long child) {
    std::set<long> candidates = getOnTreeParents(long child);
    if (candidates == NULL || candidates.isEmpty()) {
        return -1;
    }
    // return min(candidates, Cuboid.cuboidSelectComparator);
    return candidates;
}

std::set<long> CuboidScheduler::getOnTreeParents(long child) {
    std::list<AggregationGroup*> aggrs;
    std::list<AggregationGroup*> aggregationGroups = cubeDesc->getAggregationGroups();
    for (std::list<AggregationGroup*>::iterator it = aggregationGroups.begin(); it != aggregationGroups.end(); ++it) {
        if ((*it)->isOnTree(child)) {
            aggrs.push_back(agg);
        }
    }

    return getOnTreeParents(child, aggrs);
}