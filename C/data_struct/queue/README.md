# Queues for C

## Content

This library allows you to use Queues for C. It defines several types such as `queue_t` and `queue_node_t`. You can use both of them but the second one is already used by the library so you won't have to use it. Don't bother about the content of those 2 structures, you won't have to use them. Everything is managed by the library. You just have to use `queue_t`.

## Example of utilisation

```c
int main(void)
{
    queue_t my_queue;
	create_queue(&my_queue, sizeof(int)); //The queue will contain int datas but you can give anything you want

	int data1 = 1;
	enqueue(&my_queue, data1); //add data1 in the queue
	int data2 = 2;
	enqueue(&my_queue, data2); //add data2 in the queue

	int recup_data1;
	dequeue(&my_queue, &recup_data1); //get a value from the queue
	delete_queue(&my_queue); //free all the memory used by the queue
}
```
If you want to have an overview of how to use this library, you can take a look at the *Example* folder.

**Warning** Do not allocate memory for the data you add to the sack. The library manage the memory it use. If you want to use strings or array, just gives the size of the arrays or strings in `create_queue`.

## Advices

Do not forget to call `delete_queue` when you do not use the queue anymore, it will free the memory allocated by the library.

If you use arrays or strings, you should use fixed memory size (`char[10]` instead of `char *` or `int[3]` instead of `int *`). If you decide to use non fixed arrays, you will have to manage the memory yourself because the library do not manage allocated variables.

## Warnings

The size given to `create_queue` is essential. It defines what you want to store in the queue.

## Example

You can run the example by using `make` to compile and `./test` to run the tests or `make test` to compile and run the tests.

## Contributors

- [Thomas RIVES](https://github.com/ThomasRives)
