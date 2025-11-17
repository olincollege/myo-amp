from pytorch_tcn import TCN

model = TCN(
    num_inputs = 3, # 3 sensors
    num_channels = [], # default
    kernel_size = 4, # default
    dilations = None, # default
    dilation_reset = None, # default
    dropout = 0.1, # default
    causal = True, # default
    use_norm = 'weight_norm', # default
    activation = 'relu', # default
    kernel_initializer = 'xavier_uniform', # default, bai et al. paper had normal initialization
    use_skip_connections = False, # default
    input_shape = 'NCL', # default
    embedding_shapes = None, # default
    embedding_mode = 'add', # default
    use_gate = False, # default, true -> maybe better, definitely computationally costlier
    lookahead = 0, # default, deprecated
    output_projection = None, # default
    output_activation= 'softmax' # defaults to None
)
