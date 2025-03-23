class KthLargest:
    def __init__(self, k: int, nums: List[int]):
        self.k = k
        self.heap = []

        for num in nums:
            self.add(num)

    ###

    def parentIdx(self, idx):
        if (idx == 0):
            return -1
        return idx // 2
    
    def lchildIdx(self, idx):
        return idx * 2 + 1

    def rchildIdx(self, idx):
        return idx * 2 + 2

    def swap(self, a, b):
        t = self.heap[a]
        self.heap[a] = self.heap[b]
        self.heap[b] = t
    #####

    def add(self, val: int) -> int:
        self.heap.append(val)
        self.heapUp(len(self.heap)-1)

        if (len(self.heap) > self.k):
            self.pop()

        return self.heap[0]

    def heapUp(self, idx):
        pidx = self.parentIdx(idx)
        if (pidx < 0):
            return # at root
        
        if (self.heap[pidx] > self.heap[idx]):
            self.swap(pidx, idx)
            self.heapUp(pidx)

    def heapDown(self, idx):
        if (self.lchildIdx(idx) >= len(self.heap)):
            return # at leaf
        
        cidx = self.lchildIdx(idx)
        ridx = self.rchildIdx(idx)
        if (ridx < len(self.heap) and self.heap[ridx] < self.heap[cidx]):
            cidx = ridx
        
        if (self.heap[cidx] >= self.heap[idx]):
            return
        
        self.swap(cidx, idx)
        self.heapDown(cidx)

    def pop(self):
        self.heap[0] = self.heap[len(self.heap)-1]
        self.heap = self.heap[:self.k]

        self.heapDown(0)

