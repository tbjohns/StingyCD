import os
import numpy as np
import ctypes
import _ctypes
from scipy import sparse

current_dir = os.path.abspath(os.path.dirname(__file__))
lib = np.ctypeslib.load_library("cd", current_dir)

lib.CD_new.restype = ctypes.POINTER(ctypes.c_void_p)
lib.CD_set_A_dense.restype = None
lib.CD_set_A_dense.argtypes = [
    ctypes.c_void_p,
    ctypes.POINTER(ctypes.c_double),
    ctypes.c_int,
    ctypes.c_int,
    ctypes.POINTER(ctypes.c_long)]
lib.CD_delete.argtypes = [ctypes.c_void_p]
lib.CD_delete.restype = None
lib.CD_set_A_sparse.restype = None
lib.CD_set_A_sparse.argtypes = [
    ctypes.c_void_p,
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_uint),
    ctypes.POINTER(ctypes.c_uint),
    ctypes.c_uint,
    ctypes.c_uint,
    ctypes.c_uint]
lib.CD_set_b.restype = None
lib.CD_set_b.argtypes = [
    ctypes.c_void_p,
    ctypes.POINTER(ctypes.c_double),
    ctypes.c_int]
lib.CD_solve.restype = None
lib.CD_solve.argtypes = [
    ctypes.c_void_p,
    ctypes.c_double,
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_char)]
lib.CD_set_tolerance.restype = None
lib.CD_set_tolerance.argtype = [
    ctypes.c_void_p,
    ctypes.c_double]
lib.CD_set_verbose.argtype = [
    ctypes.c_void_p,
    ctypes.c_bool]
lib.CD_set_max_time.restype = None
lib.CD_set_max_time.argtype = [
    ctypes.c_void_p,
    ctypes.c_double]


def double_pointer(obj):
    return obj.ctypes.data_as(ctypes.POINTER(ctypes.c_double))


def int_pointer(obj):
    return obj.ctypes.data_as(ctypes.POINTER(ctypes.c_int))


def uint_pointer(obj):
    return obj.ctypes.data_as(ctypes.POINTER(ctypes.c_uint))


class LassoProblem:

    def __init__(self, A, b):
        self.blitzObj = lib.CD_new()
        self.reset_A_and_b(A, b)

    def reset_A_and_b(self, A, b):
        (n, d) = A.shape
        self.shape = A.shape
        if (sparse.issparse(A)):

            if (not sparse.isspmatrix_csc(A)):
                A = A.tocsc()

            # Sparse matrix, need to pass indices and values as separate arrays
            dataArg = double_pointer(A.data)
            indicesArg = uint_pointer(A.indices)
            indptrArg = uint_pointer(A.indptr)
            lib.CD_set_A_sparse(self.blitzObj, dataArg, indicesArg, indptrArg, ctypes.c_uint(
                A.nnz), ctypes.c_uint(n), ctypes.c_uint(d))

        else:

            matrixArg = double_pointer(A)
            lib.CD_set_A_dense(self.blitzObj, matrixArg, ctypes.c_int(
                n), ctypes.c_int(d), A.ctypes.strides)

        self.reset_b(b)

    def reset_b(self, b):
        n = len(b)
        self.n = len(b)
        labelsArg = double_pointer(b)
        lib.CD_set_b(self.blitzObj, labelsArg, ctypes.c_int(n))

    def set_tolerance(self, tolerance):
        lib.CD_set_tolerance(self.blitzObj, ctypes.c_double(tolerance))

    def set_verbose(self, verbose):
        lib.CD_set_verbose(self.blitzObj, ctypes.c_bool(verbose))

    def set_max_time(self, max_time):
        lib.CD_set_max_time(self.blitzObj, ctypes.c_double(max_time))

    def solve(self, lam, x0=None, log_dir=""):
        (n, d) = self.shape
        if x0 is not None:
            xstar = x0
        else:
            xstar = np.zeros(d)
        xstarArg = double_pointer(xstar)

        residuals = np.zeros(n)
        residualsArg = double_pointer(residuals)

        obj = np.array(0.)
        objArg = double_pointer(obj)

        logArg = ctypes.c_char_p(log_dir.encode('utf-8'))

        lib.CD_solve(
            self.blitzObj,
            ctypes.c_double(lam),
            xstarArg,
            residualsArg,
            objArg,
            logArg)

        def result(): return 0
        result.x = xstar
        result.residuals = residuals
        result.obj = obj.tolist()

        return result

    def __del__(self):
        lib.CD_delete(self.blitzObj)
