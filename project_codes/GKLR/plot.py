import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
type_ = 'dense'
file= 'data_5'
csv_file = type_+'/'+file+'.csv'
data = pd.read_csv('results/'+csv_file)  # Replace with your actual file path

# Ensure numeric data types
data['partition_count'] = pd.to_numeric(data['partition_count'], errors='coerce')
data['balance_difference'] = pd.to_numeric(data['balance_difference'], errors='coerce')
data['cut_size'] = pd.to_numeric(data['cut_size'], errors='coerce')
# Drop rows with NaN values
data = data.dropna(subset=['partition_count', 'balance_difference', 'cut_size'])
# Calculate mean CutSize for each PartitionCount
cut_size = data.groupby('partition_count')['cut_size'].mean()
partition_count = cut_size.index

# create folder if not exists
import os
if not os.path.exists('plots/'+type_+'/'):
    os.makedirs('plots/'+type_+'/')



# Plot PartitionCount vs CutSize
plt.figure(figsize=(10, 5))
plt.plot(partition_count, cut_size, color='blue', marker='o', label='Partition Count vs Average Cut Size')
plt.title('Partition Count vs Average Cut Size')
plt.xlabel('Partition Count')
plt.ylabel('Average Cut Size')
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.savefig('plots/'+type_+'/'+file+'_plot_1.png')
plt.show()





# Calculate mean CutSize for each BalanceDifference
cut_size2 = data.groupby('balance_difference')['cut_size'].mean()
balance = cut_size2.index

# Plot BalanceDifference vs CutSize
plt.figure(figsize=(10, 5))
plt.plot(balance, cut_size2, color='red', marker='o', label='Balance Difference vs Average Cut Size')
plt.title('Balance Difference vs Average Cut Size')
plt.xlabel('Balance Difference')
plt.ylabel('Average Cut Size')
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.savefig('plots/'+type_+'/'+file+'_plot_2.png')
plt.show()


