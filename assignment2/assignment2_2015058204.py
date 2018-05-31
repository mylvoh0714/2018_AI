import math

class Cluster:
    def __init__(self,wordNum,name):
        self.wordNum = wordNum
        self.name = name
        self.cluNum = 0

def getDotProduct(a,b):
    ret = 0.0
    for i in range(len(a)):
        ret += a[i] * b[i]
    return ret

def getNorm(a):
    ret = getDotProduct(a,a)
    ret = ret**0.5
    return ret

def getCos(a,b):
    cos = getDotProduct(a,b) / (getNorm(a) * getNorm(b))
    return cos

def getEuclidean(a,b):
    ret = 0.0
    for i in range(len(a)):
        ret += (a[i]-b[i])*(a[i]-b[i])
    ret = ret**0.5
    return ret

def getSimilarity(flag,a,b):
    if flag == 1:
        return getCos(a,b)
    elif flag == 2:
        return getEuclidean(a,b)

def main():
    print("Clustering WordEmbdding.txt ......")
    print("Write down threshold similarity: ")
    threshold = float(input())

    print("Write down type of similarity Press 1 or 2")
    print("1. Cosine Similarity")
    print("2. Euclidean Similarity")
    simil_flag = int(input())

    lines = open('WordEmbedding.txt','r').read().split('\n')

    vectors = []
    # parse input string
    for i in range(1,len(lines),2):
        word = lines[i].split(',')
        word = list(map(float,word))
        vectors.append(word)

    cluArr = []
    for i in range(len(vectors)): # i = 0 ~ 337 총 338개
        cluArr.append(Cluster(i,lines[2*i])) # Cluster(WordNum, Name)

    similTuple = []
    for i in range(len(vectors)):
        for j in range(i+1,len(vectors)):
            val = (getSimilarity(simil_flag,vectors[i],vectors[j]),i,j)
            # val = (getEuclidean(vectors[i],vectors[j]),i,j)
            similTuple.append(val)

    similTuple.sort(key=lambda t:t[0],reverse=True)

    visitedList = [] # Checked or visited List

    lenArr = len(cluArr)

    # Below, is clustering algorithm
    index = 0
    g_cluNum = 1001
    while index < (lenArr*(lenArr-1))/2 and similTuple[index][0] >= threshold:
        print('Clustering 진행중... : ', index+1 , '번째')
        A = similTuple[index][1] # in similTuple[i], syntax = {similarity, A, B}
        B = similTuple[index][2] # A.WordNum < B.WordNum

        # case of A,B both don't have clustering group
        if cluArr[A].cluNum == 0 and cluArr[B].cluNum == 0:
            cluArr[A].cluNum = cluArr[B].cluNum = g_cluNum
            g_cluNum += 1
        # case of A has clustering group, B has not
        elif cluArr[A].cluNum != 0 and cluArr[B].cluNum == 0:
            target = cluArr[A].cluNum
            flag = 1
            for i in range(1,len(cluArr)):
                if cluArr[i].cluNum == target:
                    if find(visitedList,list([A,B])) == True:
                        flag *= 1
                    else:
                        flag = 0
                        break
            if flag == 1:
                minNum = cluArr[A].cluNum if cluArr[A].cluNum < cluArr[B].cluNum else cluArr[B].cluNum
                maxNum = cluArr[A].cluNum if cluArr[A].cluNum > cluArr[B].cluNum else cluArr[B].cluNum
                for i in range(1,len(cluArr)):
                    if cluArr[i].cluNum == maxNum :
                        cluArr[i].cluNum = minNum

        # case of B has clustering group, A has not
        elif cluArr[A].cluNum == 0 and cluArr[B].cluNum != 0:
            target = cluArr[B].cluNum
            flag = 1
            for i in range(1,len(cluArr)):
                if cluArr[i].cluNum == target:
                    if find(visitedList,list([A,B])) == True:
                        flag *= 1
                    else:
                        flag = 0
                        break
            if flag == 1:
                minNum = cluArr[A].cluNum if cluArr[A].cluNum < cluArr[B].cluNum else cluArr[B].cluNum
                maxNum = cluArr[A].cluNum if cluArr[A].cluNum > cluArr[B].cluNum else cluArr[B].cluNum
                for i in range(1,len(cluArr)):
                    if cluArr[i].cluNum == maxNum :
                        cluArr[i].cluNum = minNum

        # case of A,B both have clustering group
        elif cluArr[A].cluNum != 0 and cluArr[B].cluNum != 0:
            target = cluArr[A].cluNum
            flag = 1
            for i in range(1,len(cluArr)):
                if cluArr[i].cluNum == target:
                    if find(visitedList,list([A,B])) == True:
                        flag *= 1
                    else:
                        flag = 0
                        break
            target = cluArr[B].cluNum
            for i in range(1,len(cluArr)):
                if cluArr[i].cluNum == target:
                    if find(visitedList,list([A,B])) == True:
                        flag *= 1
                    else:
                        flag = 0
                        break
            if flag == 1:
                minNum = cluArr[A].cluNum if cluArr[A].cluNum < cluArr[B].cluNum else cluArr[B].cluNum
                maxNum = cluArr[A].cluNum if cluArr[A].cluNum > cluArr[B].cluNum else cluArr[B].cluNum
                for i in range(1,len(cluArr)):
                    if cluArr[i].cluNum == maxNum :
                        cluArr[i].cluNum = minNum

        index += 1
        visitedList.append([A,B])

    # it is outside of while statement
    # for re-numbering cleanly
    g_cluNum = 1
    tmp = 0
    for i in range(lenArr):
        if cluArr[i].cluNum > 1000:
            tmp = cluArr[i].cluNum
            cluArr[i].cluNum = g_cluNum
            for j in range(i,lenArr):
                if cluArr[j].cluNum == tmp:
                    cluArr[j].cluNum = g_cluNum
            g_cluNum += 1
        elif cluArr[i].cluNum == 0:
            cluArr[i].cluNum = g_cluNum
            g_cluNum += 1
        else :
            pass

    for i in range(lenArr):
        print(cluArr[i].name, cluArr[i].cluNum)
    print('총 cluster 개수 : ', g_cluNum - 1)

    #------------- WordClustering.txt 파일 출력 부분-----------------
    f = open('WordClustering.txt','w')
    for i in range(len(lines)):
        f.write(lines[i])
        f.write("\n")
        if i%2 == 1:
            strr = str(cluArr[i//2].cluNum)
            f.write(strr)
            f.write("\n")
    f.close()

    #------------------Entropy 콘솔 출력 부분---------------------
    # lines = open('WordTopic.txt','r').read().split('\n')
    # group = MakeEntropyGroup(lines)
    # for i in range(1,g_cluNum):
    #     Totalcnt = 0
    #     for i in range(lenArr):
    #         if cluArr[i].cluNum == i :
    #             Totalcnt += 1


# def MakeEntropyGroup(lines):
#     ret = 0
#     cnt = 0
#     length = len(lines)
#     group = []
#     for i in range(length):
#         if lines[i] == "":
#             cnt = 0
#         elif lines[i][0] == '[':
#             list = []
#             group.append(list)
#         else:
#             list.append(lines[i])
#             cnt += 1
#     return group

# def getTotalEntropy(lines):
#     ret = 0
#     cnt = 0
#     length = len(lines)
#     for i in range(length):
#         if lines[i] == "":
#             ret += -(cnt/length)*math.log2(cnt/length)
#             cnt = 0
#         elif lines[i][0] == '[':
#             pass
#         else:
#            cnt += 1
#     return ret

def find(list,target):
    for i in range(len(list)):
        if list[i] == target:
            return True
    return False



if __name__ == "__main__":
    main()
