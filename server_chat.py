import socket
from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

# Load model
print("Loading model...")
model_name = "microsoft/DialoGPT-small"  # or "gpt2"
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForCausalLM.from_pretrained(model_name)
print("Model loaded")

HOST = "127.0.0.1"
PORT = 9090

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(5)
print(f"Server listening on {HOST}:{PORT}")

while True:
    conn, addr = server.accept()
    print(f"Connected by {addr}")
    with conn:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            user_in = data.decode("utf-8").strip()
            if user_in.lower() == "exit":
                conn.sendall("Goodbye!".encode())
                break

            # Encode input
            input_ids = tokenizer.encode(user_in + tokenizer.eos_token, return_tensors="pt")
            # Generate output without max_length limit
            output_ids = model.generate(input_ids, pad_token_id=tokenizer.eos_token_id)
            reply = tokenizer.decode(output_ids[0], skip_special_tokens=True)
            conn.sendall(reply.encode("utf-8"))

