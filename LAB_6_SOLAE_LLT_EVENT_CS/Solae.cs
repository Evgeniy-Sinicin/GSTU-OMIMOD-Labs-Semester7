using MathNet.Numerics.LinearAlgebra.Double;
using System;
using System.Text;

namespace LAB_6_SOLAE_LLT_EVENT_CS
{
    public class Solae
    {
        public static Random Random { get; set; } = new Random();

        public int Index { get => _index; }
        public int Size { get => _size; }

        private int _index;
        private int _size;
        private DenseMatrix _system;
        private DenseMatrix _coeffs;

        public Solae(int _size, int _index)
        {
            this._size = _size;
            this._index = _index;

            _system = new DenseMatrix(_size);
            _coeffs = new DenseMatrix(_size, 1);

            for (int i = 0; i < _size; i++)
            {
                for (int j = 0; j < _size; j++)
                {
                    _system[i, j] = Random.NextDouble() * 10;
                }

                _coeffs[i, 0] = Random.NextDouble() * 10;
            }

            _system = (DenseMatrix)_system.Multiply(_system.Transpose());
        }

        public Solae(string _solae, int _index)
        {
            this._index = _index;

            var _rows = _solae.Split("\n");
            _size = _rows.Length;
            _system = new DenseMatrix(_size);
            _coeffs = new DenseMatrix(_size, 1);

            for (int i = 0; i < _size; i++)
            {
                var _cols = _rows[i].Split("\t");

                for (int j = 0; j <= _size; j++)
                {
                    if (j < _size)
                    {
                        _system[i, j] = double.Parse(_cols[j]);
                    }
                    else
                    {
                        _coeffs[i, 0] = double.Parse(_cols[j]);
                    }
                }
            }
        }

        public Solae(DenseMatrix _system, DenseMatrix _coeffs)
        {
            if (_system.RowCount == _system.ColumnCount)
            {
                _size = _system.RowCount;
            }

            this._system = _system;
            this._coeffs = _coeffs;
        }

        public Solae(double[,] _system, double[,] _coeffs)
        {
            this._system = DenseMatrix.OfArray(_system);
            this._coeffs = DenseMatrix.OfArray(_coeffs);

            if (this._system.RowCount == this._system.ColumnCount)
            {
                _size = this._system.RowCount;
            }
        }

        public DenseMatrix Solve()
        {
            return (DenseMatrix)_system.Cholesky().Solve(_coeffs);
        }

        public string ToStringWithDecision()
        {
            var _decision = Solve();
            var _sb = new StringBuilder();

            _sb.Append("System");

            for (int i = 0; i < _size; i++)
            {
                _sb.Append("\t");
            }

            _sb.Append("Coeffs\tDecision\n");

            for (int i = 0; i < _size; i++)
            {
                for (int j = 0; j < _size; j++)
                {
                    _sb.Append($"{Math.Round(_system[i, j], 1)}\t");
                }

                _sb.Append($"{Math.Round(_coeffs[i, 0], 1)}\t");
                _sb.Append($"{Math.Round(_decision[i, 0], 1)}");

                if (i != _size - 1)
                {
                    _sb.Append("\n");
                }
            }

            return _sb.ToString();
        }

        public override string ToString()
        {
            var _sb = new StringBuilder();

            for (int i = 0; i < _size; i++)
            {
                for (int j = 0; j < _size; j++)
                {
                    _sb.Append($"{_system[i, j]}\t");
                }

                _sb.Append($"{_coeffs[i, 0]}");

                if (i != _size - 1)
                {
                    _sb.Append("\n");
                }
            }

            return _sb.ToString();
        }
    }
}
