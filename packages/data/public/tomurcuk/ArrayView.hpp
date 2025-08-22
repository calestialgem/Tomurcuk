/*
 * `ArrayView` is a reference to an array that is formed by a `ArrayPointer` and
 * a length.
 *
 * There might not be a referred array if the pointer is `nullptr`. That state
 * should work like an empty array.
 *
 * The referred portion can be shrunk from the beginning and/or the end. The
 * helper functions that provide this return a new `ArrayView` instead of
 * modifying the existing one.
 *
 *
 */
