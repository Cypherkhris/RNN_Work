import os

from keras.models import Sequential, load_model
from keras.layers import LSTM, Dropout, TimeDistributed, Dense, Activation, Embedding

MODEL_DIR = './model'

def save_weights(epoch, model,id):
    if not os.path.exists(MODEL_DIR):
        os.makedirs(MODEL_DIR)
    model.save_weights(os.path.join(MODEL_DIR, 'model'+id+'weights.{}.h5'.format(epoch)))

def load_weights(epoch, model):
    model.load_weights(os.path.join(MODEL_DIR, 'model1522879818weights.{}.h5'.format(epoch)))

def build_model(batch_size, seq_len, vocab_size):
    model = Sequential()
    model.add(Embedding(vocab_size, 256, batch_input_shape=(batch_size, seq_len)))
    for i in range(2): # changed to 2
        model.add(LSTM(32, return_sequences=True, stateful=True))# changed from 256 to 128
        model.add(Dropout(0.2))

    model.add(TimeDistributed(Dense(vocab_size)))
    model.add(Activation('softmax'))
    return model

if __name__ == '__main__':
    model = build_model(16, 64, 50)
    model.summary()
