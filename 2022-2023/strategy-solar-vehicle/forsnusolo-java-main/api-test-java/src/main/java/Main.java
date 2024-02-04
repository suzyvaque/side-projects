import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.MediaType;
import okhttp3.RequestBody;
import org.json.JSONObject;

public class Main {
    public static void main(String[] args) {

        OkHttpClient client = new OkHttpClient().newBuilder().build();

        MediaType mediaType = MediaType.parse("text/plain");
        // RequestBody body = RequestBody.create(mediaType, "");
        Request request = new Request.Builder()
                .url("https://api.solcast.com.au/data/forecast/radiation_and_weather?latitude=-33.86882&longitude=151.209295&hours=168&api_key=KrBMnoWWgDQ9eqb_AKcyRRP9FeBtS9ch&format=json")
                // .url("https://api.solcast.com.au/data/forecast/radiation_and_weather?latitude=-33.856784&longitude=151.215297&output_parameters=air_temp,dni,ghi&api_key=MXaFNmy6AFnRInH9Mbs3G6AFiBKfUtuP&format=json")
                // .method("GET", body)
                .method("GET", null)
                .build();

        // executing a http request
        try {
            // Execute the request and get the response
            Response response = client.newCall(request).execute();

            // Check if the request was successful (status code 200)
            if (response.isSuccessful()) {
                // Get the response body as a JSON string
                String responseBody = response.body().string();

                // Parse the JSON data
                JSONObject jsonData = new JSONObject(responseBody);
                System.out.println(responseBody);

                // Extract the information you need from the JSON object
                double airTemp = jsonData.getDouble("air_temp");
                double dni = jsonData.getDouble("dni");
                double ghi = jsonData.getDouble("ghi");

                // Now, you can store the data in variables or write it to a file or database
                // For example:
                System.out.println("Air Temperature: " + airTemp);
                System.out.println("Direct Normal Irradiance (DNI): " + dni);
                System.out.println("Global Horizontal Irradiance (GHI): " + ghi);
            } else {
                // Handle the case when the request was not successful
                System.out.println("Request failed with status code: " + response.code());
            }

        } catch (Exception e) {
            // Handle exceptions, if any
            e.printStackTrace();
        }
    }
}
