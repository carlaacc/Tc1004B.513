import firebase_admin
from firebase_admin import firestore
from firebase_admin import credentials

#Application Default credentials are automatically created
cred = credentials.Certificate('credentials.json')
app = firebase_admin.initialize_app(cred)
db = firestore.client()

users_ref = db.collection(u'sensor')
docs = users_ref.stream()

for doc in docs:
    print(f'{doc.id} => {doc.to_dict()}')
