#! /usr/bin/env python

import argparse
import os
import json

import numpy as np

from model import build_model, load_weights

from keras.models import Sequential, load_model
from keras.layers import LSTM, Dropout, TimeDistributed, Dense, Activation, Embedding

DATA_DIR = './data'
MODEL_DIR = './model'

def build_sample_model(vocab_size):
    model = Sequential()
    model.add(Embedding(vocab_size, 256, batch_input_shape=(1, 1)))
    for i in range(2):
        model.add(LSTM(32, return_sequences=(i != 1), stateful=True))
        model.add(Dropout(0.2))

    model.add(Dense(vocab_size))
    model.add(Activation('softmax'))
    return model

""""def sample(epoch, header, num_chars):
    with open(os.path.join(DATA_DIR, 'char_to_idxi1522879818.json'),'rb') as f:
        print(f)
        char_to_idx = json.load(f)
    idx_to_char = { i: ch for (ch, i) in char_to_idx.items() }
    vocab_size = len(char_to_idx)

    model = build_sample_model(vocab_size)
    load_weights(epoch, model)
    model.save(os.path.join(MODEL_DIR, 'model1522879818weights.{}.h5'.format(epoch)))

    sampled = [char_to_idx[c] for c in header]
    print( sampled)
    for c in header[:-1]:
        batch = np.zeros((1, 1))
        batch[0, 0] = char_to_idx[c]
        model.predict_on_batch(batch)

    for i in range(num_chars):
        batch = np.zeros((1, 1))

        if sampled:
            batch[0, 0] = sampled[-1]
          #  print( sampled[-1])
        else:
            batch[0, 0] = np.random.randint(vocab_size)
        result = model.predict_on_batch(batch).ravel()
        sample = np.random.choice(range(vocab_size), p=result)
        sampled.append(sample)

    print(''.join(idx_to_char[c] for c in sampled))"""
def sample(epoch, num_chars):
    with open(os.path.join(DATA_DIR, 'char_to_idxi1522879818.json'),'rb') as f:
        print(f)
        char_to_idx = json.load(f)
    idx_to_char = { i: ch for (ch, i) in char_to_idx.items() }
    vocab_size = len(char_to_idx)

    model = build_sample_model(vocab_size)
    load_weights(epoch, model)
    model.save(os.path.join(MODEL_DIR, 'model1522879818weights.{}.h5'.format(epoch)))
    li = " "
    treat = " "
    file = open(os.path.join(DATA_DIR,"result.csv"),"w") 
    with open ( os.path.join(DATA_DIR, args.seed) , 'r' ) as myfile:
        for line in myfile:
            #li = line.rstrip()
            vals = line.split("#")
            li = vals[1]
            treat = vals[0] + "#"
            sampled = [char_to_idx[c] for c in treat]
            for c in treat[:-1]:
                batch = np.zeros((1, 1))
                batch[0, 0] = char_to_idx[c]
                model.predict_on_batch(batch)
                        
            for i in range(num_chars):
                batch = np.zeros((1, 1))
                if sampled:
                    batch[0, 0] = sampled[-1]
                else:
                    batch[0, 0] = np.random.randint(vocab_size)
                result = model.predict_on_batch(batch).ravel()
                sample = np.random.choice(range(vocab_size), p=result)
                sampled.append(sample)
                #print(idx_to_char[c] for c in sampled)
            generated = ''.join(idx_to_char[c] for c in sampled)
            values = generated.split("#")
            print(generated, end='$')
            file.write(values[0]+", "+values[1] + ", "+ li  +  "\n")

#
# pseudo-main
#
parser = argparse.ArgumentParser(description='Sample some text from the trained model.')
parser.add_argument('epoch', type=int, help='epoch checkpoint to sample from')
parser.add_argument('--seed', default='methode20', help='initial seed for the generated text')
#parser.add_argument('--seed', default='^{ writeByte(value); writeByte(value >> 8); writeByte(value >> 16); }#', help='initial seed for the generated text')
parser.add_argument('--len', type=int, default=20, help='number of characters to sample (default 512)')
args = parser.parse_args()

print("start sampling\n")
sample(args.epoch, args.len)
#sample(args.epoch, args.seed, args.len)
