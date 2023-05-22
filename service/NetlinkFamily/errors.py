class AttributeNotExists(Exception):
    """
        The error means that the program tried to access an attribute that doens't exists.
    """

    def __init__(self):
        self.message = "The attribute you tried to find doesn't exists."
        super().__init__(self.message)

