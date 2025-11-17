from pytorch_tcn import TCN

model = TCN( 
    num_inputs: 3, # based on # sensors we have
    num_channels: ArrayLike,
    kernel_size: int = 4,
    dilations: Optional[ ArrayLike ] = None,
    dilation_reset: Optional[ int ] = None,
    dropout: float = 0.1,
    causal: bool = True,
    use_norm: str = 'weight_norm',
    activation: str = 'relu',
    kernel_initializer: str = 'xavier_uniform',
    use_skip_connections: bool = False,
    input_shape: str = 'NCL',
    embedding_shapes: Optional[ ArrayLike ] = None,
    embedding_mode: str = 'add',
    use_gate: bool = False,
    lookahead: int = 0,
    output_projection: Optional[ int ] = None,
    output_activation: Optional[ str ] = None,
    )
# Continue to train/use model for your task