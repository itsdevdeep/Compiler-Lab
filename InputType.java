
import java.util.Scanner;

public class InputType {
    public static void main(String[] args) {
//        int a = 5;
//        System.out.println(((Object) a).getClass().getSimpleName());
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter value for variable a: ");
        Integer a = sc.nextInt();
        if (a.getClass().getSimpleName().equals("Integer")) {
            System.out.println("No error");
        } else {
            System.out.println("Type Mismatch");
        }

    }
}
