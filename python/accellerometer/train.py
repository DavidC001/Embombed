import numpy as np
from sklearn import tree
from sklearn import neural_network

#dati da csv formato X Y Z (input) class (predizione)
data = np.genfromtxt('data.csv', delimiter=',')
X = data[1:][:,0]
#Y = data[1:][:,1]
Z = data[1:][:,2]

#crea l'array di input X[0]Y[0]Z[0] X[1]Y[1]Z[1] ecc
#input_array = np.array([X,Y,Z]).T
input_array = np.array([X,Z]).T
class_train = data[1:][:,3]

#split train test
def split_train_test(input_array, class_train, train_percent):
    train_size = int(len(input_array)*train_percent)
    return input_array[:train_size], input_array[train_size:], class_train[:train_size], class_train[train_size:]

#shuffle
np.random.seed(0)
np.random.shuffle(input_array)
np.random.seed(0)
np.random.shuffle(class_train)
input_array_train, input_array_test, class_train_train, class_train_test = split_train_test(input_array, class_train, 0.8)

def dt():
    print("train")
    #crea il modello
    clf = tree.DecisionTreeClassifier(max_depth=4)
    clf.fit(input_array_train, class_train_train)
    print("accurancy: "+str(clf.score(input_array_train, class_train_train)))
    print("test")
    print("accurancy: "+str(clf.score(input_array_test, class_train_test)))
    print("save")
    #salva il modello
    tree.export_graphviz(clf, out_file='tree.dot')

print("TREE CLASSIFIER")
dt()
