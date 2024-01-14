# A backtesting engine for HFT strategies

## About
HFTBacktest is an engine built for backtesting strategies on CME futures. It works on MBO data from [Databento](https://databento.com).

## Features 
- Full orderbook reconstruction using MBO data
- Realistic fill simulation using position in queue
- Latency modelling
- Fast, written fully in C++
- Python bindings for strategy research (coming) 

## Motivation 
I develop futures trading strategies as a hobby, and I was tired of my live results not matching my backtests. This was especially true for latency sensitive and maker (limit order) strategies. I built HFTBacktest to accurately simulate trading fills