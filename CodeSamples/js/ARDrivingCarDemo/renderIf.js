/**
 * Helper function for conditional rendering. Takes two arguments:
 * condition - any condition that evaluates to true or false,
 * content - JS content to render if the give condition evaluates to true
 */
export default function renderIf(condition, content) {
    if (condition) {
        return content;
    } else {
        return null;
    }
}
